#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <WiFi.h>
#include <Wire.h>
#include <Preferences.h>
#include <time.h>
#include <TAMC_GT911.h>
extern "C" {
#include "can_platform.h"
#include "can_node_panel_bus0.h"
}
#include "screen.h"
#include "src/ui/ui.h" // Подключение сгенерированного UI
#include "src/ui/actions.h"
#include "src/ui/screens.h"
#include "src/ui/vars.h"

#include "esp_timer.h"

#define DS18B20_USED
#ifdef DS18B20_USED
#include "ds18b20.h"
#endif

volatile t_can_node_panel_bus0_input can_in;
volatile t_can_node_panel_bus0_output can_out;

const char* ntpServer = "pool.ntp.org";
const char* timeZone = "MSK-3";

volatile float temperatureSelf = DEVICE_DISCONNECTED_C;

// === Native global variable ===
char ip_address[] = "---.---.---.---";
char temperature_str[20] = { 0 };
char power_str[20] = { 0 };
char time_str[10] = { 0 };
status_color can_status = status_color_GREY;
status_color wifi_status = status_color_GREY;
status_color bt_status = status_color_GREY;

int status_bar_update_req = 0;

Preferences prefs;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ЗАПУСК ПЛАТЫ С ARDUINO_RGB_DISPLAY ===");
  // Инициализация CAN
  platform_can_init();
  can_node_panel_bus0_init(0, 0, 0, &can_out, &can_in);
  can_timer_init();
  loadLastState();
  
  // Инициализация экрана
  screen_init();
  // Инициализация LVGL
  lv_init();
  lv_tick_set_cb(millis);
  // Регистрация экрана
  lv_display_t * display = lv_display_create(480, 480);
  lv_display_set_buffers(display, disp_draw_buf, NULL, DISP_DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(display, my_disp_flush);
  // Регистрация тачскрина в LVGL
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  // Запуск графического интерфейса EEZ
  ui_init();
  // Принудительно забираем StatusBar у экрана и отдаем его верхнему слою LVGL
  lv_obj_set_parent(objects.status_bar, lv_layer_top()); 
  lv_obj_set_parent(objects.background_image, lv_layer_bottom()); 
  screen_timer_init();

  WiFi.begin();
  configTzTime(timeZone, ntpServer);

  tempSensorInit();
  // pinMode(40, OUTPUT); // Настраиваем 40-й пин как выход

  Serial.println("🎉 Система успешно запущена!");
}

// === Основное Loop цикл ===

void loop() {
  status_update_poll();

  lv_timer_handler_run_in_period(10);
  ui_tick();

  if(platform_can_poll(0))
  {
    //CAN ACTIVE
  }
  can_node_panel_bus0_rx(&can_in);
  can_node_panel_bus0_tx(&can_out);

  saveCurrentStatePoll();

  // tempSensorPoll(&temperatureSelf);
//   digitalWrite(40, HIGH); // Включаем (3.3В)
//   delay(50);                        // Ждем полсекунды
//   digitalWrite(40, LOW);  // Выключаем (0В)
//   delay(50);
}

// === ОБРАБОТЧИКИ ДЕЙСТВИЙ ===
void action_connect_wifi(lv_event_t *e) {
  WiFi.disconnect(true);
  while (WiFi.status() == WL_CONNECTED) {
    delay(100);
  }
  WiFi.begin(lv_textarea_get_text(objects.ssid_text), lv_textarea_get_text(objects.password_text));
  // Serial.println(lv_textarea_get_text(objects.ssid_text));
  // Serial.println(lv_textarea_get_text(objects.password_text));
}

// === Обновление глобальных переменных статуса ===
void status_update_poll(void)
{
  if(status_bar_update_req)
  {
    status_bar_update_req = 0;
    // Время
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    if(timeinfo.tm_year > (2020 - 1900))
    {
      strftime(time_str, sizeof(time_str), "%H:%M", &timeinfo);
    }
    else
    {
      strcpy(time_str, "--:--");
    }

    // WIFI
    switch(WiFi.status())
    {
      case WL_CONNECTED: set_var_wifi_status(status_color_GREEN); break;
      case WL_DISCONNECTED : set_var_wifi_status(status_color_GREY); break;
      case WL_IDLE_STATUS : set_var_wifi_status(status_color_YELLOW); break;
      case WL_CONNECTION_LOST  : set_var_wifi_status(status_color_RED); break;
      default : set_var_wifi_status(status_color_WHITE); break;
    }

    // Bluetooth

    // IP address
    set_var_ip_address(WiFi.localIP().toString().c_str());

    // CAN
    if(can_in.alive.valve_status && can_in.alive.power_status && can_in.alive.fan_status) set_var_can_status(status_color_GREEN);
    else if (!can_in.alive.valve_status && !can_in.alive.power_status && !can_in.alive.fan_status) set_var_can_status(status_color_RED);
    else set_var_can_status(status_color_YELLOW);

    if(can_in.alive.power_status)
    {
      sprintf(power_str, "%.1fV %.2fA %.1fW", can_in.POWER_STATUS.V_12V, can_in.POWER_STATUS.I_12V, can_in.POWER_STATUS.P_12V);
    }
    else
    {
      sprintf(power_str, "No power data");
    }

    // Temperature
#ifdef DS18B20_USED
  if (temperatureSelf > -126.0f)
    sprintf(temperature_str, "%.2f°C", temperatureSelf);
  else
    sprintf(temperature_str, "--°C");
#else
#endif
  }
}

// Native global variables
status_color get_var_can_status() {
    return can_status;
}

void set_var_can_status(status_color value) {
    can_status = value;
}

status_color get_var_wifi_status() {
    return wifi_status;
}

void set_var_wifi_status(status_color value) {
    wifi_status = value;
}
status_color get_var_bt_status() {
    return bt_status;
}

void set_var_bt_status(status_color value) {
    bt_status = value;
}

const char *get_var_ip_address() {
    return ip_address;
}

void set_var_ip_address(const char *value) {
    strncpy(ip_address, value, sizeof(ip_address) / sizeof(char));
    ip_address[sizeof(ip_address) / sizeof(char) - 1] = 0;
}

const char *get_var_time_str() {
    return time_str;
}

void set_var_time_str(const char *value) {
    strncpy(time_str, value, sizeof(time_str) / sizeof(char));
    time_str[sizeof(time_str) / sizeof(char) - 1] = 0;
}

const char *get_var_power_str() {
    return power_str;
}

void set_var_power_str(const char *value) {
    strncpy(power_str, value, sizeof(power_str) / sizeof(char));
    power_str[sizeof(power_str) / sizeof(char) - 1] = 0;
}

const char *get_var_temperature_str() {
    return temperature_str;
}

void set_var_temperature_str(const char *value) {
    strncpy(temperature_str, value, sizeof(temperature_str) / sizeof(char));
    temperature_str[sizeof(temperature_str) / sizeof(char) - 1] = 0;
}

// int32_t exhaust_fan_speed;

int32_t get_var_exhaust_fan_speed() {
    return can_out.CTRL_FAN.FAN_1_REQ;
}

void set_var_exhaust_fan_speed(int32_t value) {
    can_out.CTRL_FAN.FAN_1_REQ = value;
    notifyChageValue();
}

// int32_t supply_fan_speed;

int32_t get_var_supply_fan_speed() {
    return can_out.CTRL_FAN.FAN_2_REQ;
}

void set_var_supply_fan_speed(int32_t value) {
    can_out.CTRL_FAN.FAN_2_REQ = value;
    notifyChageValue();
}

// int32_t family_room_valve;

int32_t get_var_family_room_valve() {
    return can_out.CTRL_VALVE.VALVE_1_REQ;
}

void set_var_family_room_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_1_REQ = value;
    notifyChageValue();
}

// int32_t master_badroom_valve;

int32_t get_var_master_badroom_valve() {
    return can_out.CTRL_VALVE.VALVE_2_REQ;
}

void set_var_master_badroom_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_2_REQ = value;
    notifyChageValue();
}

// int32_t children_room_valve;

int32_t get_var_children_room_valve() {
    return can_out.CTRL_VALVE.VALVE_3_REQ;
}

void set_var_children_room_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_3_REQ = value;
    notifyChageValue();
}

// int32_t pantry_room_valve;

int32_t get_var_pantry_room_valve() {
    return can_out.CTRL_VALVE.VALVE_4_REQ;
}

void set_var_pantry_room_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_4_REQ = value;
    notifyChageValue();
}

// int32_t toilet_exhaust_valve;

int32_t get_var_toilet_exhaust_valve() {
    return can_out.CTRL_VALVE.VALVE_5_REQ < 50 ? 0 : 1;
}

void set_var_toilet_exhaust_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_5_REQ = value ? 100 : 0;
    notifyChageValue();
}

// int32_t bathroom_exhaust_valve;

int32_t get_var_bathroom_exhaust_valve() {
    return can_out.CTRL_VALVE.VALVE_6_REQ < 50 ? 0 : 1;
}

void set_var_bathroom_exhaust_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_6_REQ = value ? 100 : 0;
    notifyChageValue();
}

int32_t brightness;

int32_t get_var_brightness() {
    return brightness;
}

void set_var_brightness(int32_t value) {
    brightness = value;
    set_brightness(brightness);
    notifyChageValue();
}

// === СОХРАНЕНИЕ И ЗАГРУЗКА СОСТОЯНИЯ ===
uint32_t lastChangingValue = 0;

void loadLastState(void)
{
  prefs.begin("actual_state");
  can_out.CTRL_VALVE.VALVE_1_REQ = prefs.getUChar("valve_1", 50);
  can_out.CTRL_VALVE.VALVE_2_REQ = prefs.getUChar("valve_2", 50);
  can_out.CTRL_VALVE.VALVE_3_REQ = prefs.getUChar("valve_3", 50);
  can_out.CTRL_VALVE.VALVE_4_REQ = prefs.getUChar("valve_4", 50);
  can_out.CTRL_VALVE.VALVE_5_REQ = prefs.getUChar("valve_5", 100);
  can_out.CTRL_VALVE.VALVE_6_REQ = prefs.getUChar("valve_6", 100);
  can_out.CTRL_FAN.FAN_1_REQ     = prefs.getUChar("fan_1", 50);
  can_out.CTRL_FAN.FAN_2_REQ     = prefs.getUChar("fan_2", 50);

  lastChangingValue = millis();
}

void saveCurrentStatePoll(void)
{
  if(millis() - lastChangingValue > 10000)
  {
    prefs.putUChar("valve_1", can_out.CTRL_VALVE.VALVE_1_REQ);
    prefs.putUChar("valve_2", can_out.CTRL_VALVE.VALVE_2_REQ);
    prefs.putUChar("valve_3", can_out.CTRL_VALVE.VALVE_3_REQ);
    prefs.putUChar("valve_4", can_out.CTRL_VALVE.VALVE_4_REQ);
    prefs.putUChar("valve_5", can_out.CTRL_VALVE.VALVE_5_REQ);
    prefs.putUChar("valve_6", can_out.CTRL_VALVE.VALVE_6_REQ);
    prefs.putUChar("fan_1", can_out.CTRL_FAN.FAN_1_REQ);
    prefs.putUChar("fan_2", can_out.CTRL_FAN.FAN_2_REQ);

    lastChangingValue = millis();
  }
}

void notifyChageValue(void)
{
  lastChangingValue = millis();
}

// === Функции таймеров ===
void can_timer_init(void)
{
  const esp_timer_create_args_t can_timer_args = {
    .callback = &can_timer_update,  // Указываем нашу функцию
    .name = "1 ms CAN timer"        // Имя таймера для отладки
  };
  esp_timer_handle_t can_timer;
  esp_timer_create(&can_timer_args, &can_timer);
  esp_timer_start_periodic(can_timer, 1000);
}

void can_timer_update(void *arg)
{
  can_node_panel_bus0_update_timers(1000);
}

void screen_timer_init(void)
{
  const esp_timer_create_args_t screen_timer_args = {
    .callback = &screen_timer_update,  // Указываем нашу функцию
    .name = "100 ms Screen timer"      // Имя таймера для отладки
  };
  esp_timer_handle_t screen_timer;
  esp_timer_create(&screen_timer_args, &screen_timer);
  esp_timer_start_periodic(screen_timer, 100000);
}

void screen_timer_update(void *arg)
{
  status_bar_update_req = 1;
}

//TODO delete

void action_valve_value_changed(lv_event_t *e) {
  // Получаем наше число из user_data
    int32_t valve_id = (int32_t)(intptr_t)lv_event_get_user_data(e);
    lv_obj_t* object = (lv_obj_t*)lv_event_get_target(e);
    
    switch(valve_id)
    {
      case 1:
        can_out.CTRL_VALVE.VALVE_1_REQ = lv_slider_get_value(object);
        break;
      case 2:
        can_out.CTRL_VALVE.VALVE_2_REQ = lv_slider_get_value(object);
        break;
      case 3:
        can_out.CTRL_VALVE.VALVE_3_REQ = lv_slider_get_value(object);
        break;
      case 4:
        can_out.CTRL_VALVE.VALVE_4_REQ = lv_slider_get_value(object);
        break;
      case 5:
        can_out.CTRL_VALVE.VALVE_5_REQ = lv_obj_has_state(object, LV_STATE_CHECKED) ? 100 : 0;
        break;
      case 6:
        can_out.CTRL_VALVE.VALVE_6_REQ = lv_obj_has_state(object, LV_STATE_CHECKED) ? 100 : 0;
        break;
      //TODO FAN control
      case 0:
        can_out.CTRL_FAN.FAN_1_REQ = lv_slider_get_value(object);
        can_out.CTRL_FAN.FAN_2_REQ = lv_slider_get_value(object);
    }
    Serial.printf("Изменено значение клапана %d \n", valve_id);
}


