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

// #include "ds18b20.h"

volatile t_can_node_panel_bus0_input can_in;
volatile t_can_node_panel_bus0_output can_out;

const char* ntpServer = "pool.ntp.org";
const char* timeZone = "MSK-3";

int status_bar_update_req = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ЗАПУСК ПЛАТЫ С ARDUINO_RGB_DISPLAY ===");
  // Инициализация CAN
  platform_can_init();
  can_node_panel_bus0_init(0, 0, 0, &can_out, &can_in);
  can_timer_init();
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
  screen_timer_init();

  WiFi.begin();
  configTzTime(timeZone, ntpServer);

  Serial.println("🎉 Система успешно запущена!");
}

void loop() {
  if(status_bar_update_req)
  {
    status_bar_update_req = 0;
    time_t now;
    struct tm timeinfo;
    char timeBuffer[6]; // Строка для "ЧЧ:ММ\0"
    time(&now);
    localtime_r(&now, &timeinfo);

    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", &timeinfo);
    // Serial.println(timeBuffer);
    lv_label_set_text(objects.status_bar__time, timeBuffer);

    switch(WiFi.status())
    {
      case WL_CONNECTED: lv_obj_set_style_text_color(objects.status_bar__wi_fi, lv_color_hex(0x00FF00), LV_PART_MAIN); break;
      case WL_DISCONNECTED : lv_obj_set_style_text_color(objects.status_bar__wi_fi, lv_color_hex(0x333333), LV_PART_MAIN); break;
      case WL_IDLE_STATUS : lv_obj_set_style_text_color(objects.status_bar__wi_fi, lv_color_hex(0xFFFF00), LV_PART_MAIN); break;
      case WL_CONNECTION_LOST  : lv_obj_set_style_text_color(objects.status_bar__wi_fi, lv_color_hex(0xFF0000), LV_PART_MAIN); break;
      default : lv_obj_set_style_text_color(objects.status_bar__wi_fi, lv_color_hex(0x000000), LV_PART_MAIN); break;
    }
  }

  lv_timer_handler_run_in_period(10);
  ui_tick();

  if(platform_can_poll(0))
  {
    //CAN ACTIVE
  }
  can_node_panel_bus0_rx(&can_in);
  can_node_panel_bus0_tx(&can_out);
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
    }
    Serial.printf("Изменено значение клапана %d \n", valve_id);
}


// Native global variables

int32_t get_var_can_status() {
  return 1;
}
void set_var_can_status(int32_t value) {}
int32_t get_var_wifi_status() {
  return 1;
}
void set_var_wifi_status(int32_t value) {}
int32_t get_var_bt_status() {
  return 1;
}
void set_var_bt_status(int32_t value) {}

// === ОБРАБОТЧИК СВАЙПОВ ===
// void action_to_mian(lv_event_t *e) {
//   // Получаем направление свайпа
//   lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
//   loadScreen(SCREEN_ID_MAIN);
//   switch(dir) {
//     case LV_DIR_LEFT:
//       loadScreen(SCREEN_ID_MAIN);
//       break;
      
//     case LV_DIR_RIGHT:
//       loadScreen(SCREEN_ID_MAIN);
//       break;
      
//     case LV_DIR_BOTTOM:
//       Serial.println("Свайп сверху вниз");
//       break;
      
//     case LV_DIR_TOP:
//       Serial.println("Свайп снизу вверх");
//       break;
//   }
// }

// Функции таймеров
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

