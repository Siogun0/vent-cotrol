#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <AsyncTCP.h>
#include <Wire.h>
#include <esp_wifi.h>
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
#include "xcp_bridge.h"

#define DS18B20_USED
// #define DHT22_USED

#ifdef DS18B20_USED
#include "ds18b20.h"
#elif defined(DHT22_USED)
#include "dht22.h"
#endif

volatile t_can_node_panel_bus0_input can_in;
volatile t_can_node_panel_bus0_output can_out;

const char* ntpServer = "pool.ntp.org";
const char* timeZone = "MSK-3";

volatile float temperatureSelf = -127.0f;
volatile float humiditySelf = -1.0f;

// === Native global variable ===
char ip_address[] = "---.---.---.---";
char temperature_str[20] = { 0 };
char power_str[20] = { 0 };
char time_str[10] = { 0 };
status_color can_status = status_color_GREY;
status_color wifi_status = status_color_GREY;
status_color bt_status = status_color_GREY;

int status_bar_update_req = 0;

XcpBridge* xcpBridge_vent;
XcpBridge* xcpBridge_vent1;
Preferences prefs;

char wifi_ssid[32] = { 0 };
char wifi_password[64] = { 0 };

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

  // Wi-Fi
  WiFi.mode(WIFI_STA); 
  wifi_config_t conf;
  esp_err_t res = esp_wifi_get_config(WIFI_IF_STA, &conf);
  strncpy(wifi_ssid, (char*)conf.sta.ssid, sizeof(wifi_ssid) / sizeof(char));
  strncpy(wifi_password, (char*)conf.sta.password, sizeof(wifi_password) / sizeof(char));
  WiFi.begin();

  configTzTime(timeZone, ntpServer);

  tempSensorInit();

  // TCP for XCP
  xcpBridge_vent = new XcpBridge(XCP_UDP_TCP, 18001, 20001, 0x7AE, 0x7AF);
  platform_can_init_rx_mb(0, MBN_NEXT_FREE_BUS_0, 0x7AF, 8);

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

// TCP for XCP костыль
  if(platform_can_is_message_arrived(0, MBN_NEXT_FREE_BUS_0))
	{
		uint64_t msg = platform_can_get_mb_data(0, MBN_NEXT_FREE_BUS_0);
    twai_message_t twai_message;
    twai_message.identifier = 0x7AF;
    twai_message.extd = 0;
	  twai_message.rtr = 0;
    twai_message.data_length_code = 8;
    twai_message.data[0] = (msg >> 0) & 0xFF;
    twai_message.data[1] = (msg >> 8) & 0xFF;
    twai_message.data[2] = (msg >> 16) & 0xFF;
    twai_message.data[3] = (msg >> 24) & 0xFF;
    twai_message.data[4] = (msg >> 32) & 0xFF;
    twai_message.data[5] = (msg >> 40) & 0xFF;
    twai_message.data[6] = (msg >> 48) & 0xFF;
    twai_message.data[7] = (msg >> 56) & 0xFF;
    if (xcpBridge_vent)
		  xcpBridge_vent->receiveCanPacket(twai_message);
	}
}

// === ОБРАБОТЧИКИ ДЕЙСТВИЙ ===
void action_connect_wifi(lv_event_t *e) {
  WiFi.disconnect(true);
  while (WiFi.status() == WL_CONNECTED) {
    delay(100);
  }
  WiFi.begin(wifi_ssid, wifi_password);
}

void action_reset_request(lv_event_t *e) {
    esp_restart();
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
#elif defined(DHT22_USED)
  if (temperatureSelf > -126.0f)
    sprintf(temperature_str, "%.1f°C %.0f%%", temperatureSelf, humiditySelf);
  else
    sprintf(temperature_str, "--°C --%%");
#else
  sprintf("");
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
    can_out.CTRL_FAN.FAN_1_REQ = constrain(value, 0, 255);
    notifyChageValue();
}

// int32_t supply_fan_speed;

int32_t get_var_supply_fan_speed() {
    return can_out.CTRL_FAN.FAN_2_REQ;
}

void set_var_supply_fan_speed(int32_t value) {
    can_out.CTRL_FAN.FAN_2_REQ = constrain(value, 0, 255);
    notifyChageValue();
}

// int32_t family_room_valve;

int32_t get_var_family_room_valve() {
    return can_out.CTRL_VALVE.VALVE_1_REQ;
}

void set_var_family_room_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_1_REQ = constrain(value, 0, 255);
    notifyChageValue();
}

// int32_t master_badroom_valve;

int32_t get_var_master_badroom_valve() {
    return can_out.CTRL_VALVE.VALVE_2_REQ;
}

void set_var_master_badroom_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_2_REQ = constrain(value, 0, 255);
    notifyChageValue();
}

// int32_t children_room_valve;

int32_t get_var_children_room_valve() {
    return can_out.CTRL_VALVE.VALVE_3_REQ;
}

void set_var_children_room_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_3_REQ = constrain(value, 0, 255);
    notifyChageValue();
}

// int32_t pantry_room_valve;

int32_t get_var_pantry_room_valve() {
    return can_out.CTRL_VALVE.VALVE_4_REQ;
}

void set_var_pantry_room_valve(int32_t value) {
    can_out.CTRL_VALVE.VALVE_4_REQ = constrain(value, 0, 255);
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

int32_t brightness = 255;

int32_t get_var_brightness() {
    return brightness;
}

void set_var_brightness(int32_t value) {
    brightness = constrain(value, 0, 255);
    set_brightness(brightness);
    notifyChageValue();
}

int32_t brightness_idle = 128;

int32_t get_var_brightness_idle() {
    return brightness_idle;
}

void set_var_brightness_idle(int32_t value) {
    brightness_idle = constrain(value, 0, 255);
    notifyChageValue();
}

int32_t idle_timeout = 5;

int32_t get_var_idle_timeout() {
    return idle_timeout;
}

void set_var_idle_timeout(int32_t value) {
    idle_timeout = value;
    notifyChageValue();
}

bool idle_is_active = false;

bool get_var_idle_is_active() {
  if (lv_display_get_inactive_time(NULL) > idle_timeout * 1000)
  {
    if (!idle_is_active)
    {
      set_brightness(brightness_idle);
    }
    idle_is_active = true;
  }
  else
  {
    if (idle_is_active)
    {
      set_brightness(brightness);
    }
    idle_is_active = false;
  }
  return idle_is_active;
}

void set_var_idle_is_active(bool value) {
    // idle_is_active = value;
    Serial.println("Idle is active");
}

const char *get_var_wifi_ssid() {
    return wifi_ssid;
}

void set_var_wifi_ssid(const char *value) {
    strncpy(wifi_ssid, value, sizeof(wifi_ssid) / sizeof(char));
    wifi_ssid[sizeof(wifi_ssid) / sizeof(char) - 1] = 0;
}

const char *get_var_wifi_password() {
    return wifi_password;
}

void set_var_wifi_password(const char *value) {
    strncpy(wifi_password, value, sizeof(wifi_password) / sizeof(char));
    wifi_password[sizeof(wifi_password) / sizeof(char) - 1] = 0;
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

  prefs.begin("settings");
  brightness = prefs.getUChar("brightness", 255);
  brightness_idle = prefs.getUChar("brightness_idle", 100);
  idle_timeout = prefs.getInt("idle_timeout", 30);

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

    prefs.putUChar("brightness", brightness);
    prefs.putUChar("brightness_idle", brightness_idle);
    prefs.putInt("idle_timeout", idle_timeout);

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



