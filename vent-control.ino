#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <Wire.h>
#include <TAMC_GT911.h>
extern "C" {
#include "can_platform.h"
#include "can_node_panel_bus0.h"
}
#include "src/ui/ui.h" // Подключение сгенерированного UI
#include "src/ui/actions.h"
#include "src/ui/screens.h"

volatile t_can_node_panel_bus0_input can_in;
volatile t_can_node_panel_bus0_output can_out;
unsigned long time_stamp;

// --- УПРАВЛЕНИЕ ПОДСВЕТКОЙ через PWM (LEDC) ---
#define GFX_BL 38      // Пин, подключенный к подсветке
#define PWM_FREQ 1000  // Частота ШИМ-сигнала (1 кГц)
#define PWM_BITS 8

// --- Пины для тачскрина GT911 ---
#define TOUCH_SDA  19
#define TOUCH_SCL  45
#define TOUCH_INT  41
#define TOUCH_RST  -1  // На этой плате пин сброса тача часто не разведен отдельно

// Размер буфера для экрана
#define DISP_DRAW_BUF_SIZE (2 * 480 * 40)

// Создаем объект тачскрина (Ширина, Высота, Пины)
TAMC_GT911 ts = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, 480, 480);

Arduino_SWSPI rgb_cmd_bus = Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC не используется */,
    39 /* CS: выбор чипа (chip select) */,
    48 /* SCK: тактовый сигнал программного SPI (clock) */,
    47 /* MOSI: данные программного SPI */,
    GFX_NOT_DEFINED /* MISO не используется */
  );

Arduino_ESP32RGBPanel rgb_panel = Arduino_ESP32RGBPanel(
  /* DE, VSYNC, HSYNC, PCLK */ 18, 17, 16, 21,
  /* R0..R4 */ 11, 12, 13, 14, 0,
  /* G0..G5 */ 8, 20, 3, 46, 9, 10,
  /* B0..B4 */ 4, 5, 6, 7, 15,
  /* hsync_pol, hfp, hpw, hbp */ 1, 10, 8, 50,  // горизонтальные параметры
  /* vsync_pol, vfp, vpw, vbp */ 1, 10, 8, 20,  // вертикальные параметры
  /* pclk_active_neg */ 0,                      // полярность PCLK (тактового сигнала пикселей)
  /* prefer_speed */ 16000000,                  // частота тактового сигнала пикселей
  /* big_endian */ false,                       // стандартный порядок байт
  /* de_idle_high, pclk_idle_high, bounce_buf */ 0, 0, 0
  );

Arduino_RGB_Display gfx = Arduino_RGB_Display(
    480, 480,                   // Разрешение панели
    &rgb_panel,                  // Интерфейс RGB
    0 /* rotation */,           // Без поворота экрана
    true /* auto_flush */,      // Автоматическое обновление фреймбуфера
    &rgb_cmd_bus,                // Шина команд
    GFX_NOT_DEFINED /* RST */,  // Отдельный пин сброса (reset) не назначен
    st7701_type9_init_operations, sizeof(st7701_type9_init_operations)
  );

// static uint8_t *disp_draw_buf = nullptr;
uint8_t disp_draw_buf[DISP_DRAW_BUF_SIZE] = {};

// Функция отрисовки фрейма
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  gfx.draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);

  lv_display_flush_ready(disp);
}

// --- Функция чтения координат тачскрина для LVGL v9 ---
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  ts.read(); // Опрашиваем чип GT911

  if (ts.isTouched) {
    data->state = LV_INDEV_STATE_PRESSED;
    
    // Получаем координаты первой точки касания
    data->point.x = ts.points[0].x;
    data->point.y = ts.points[0].y;
    
    // Опционально: вывод координат в консоль для проверки зеркальности
    // Serial.printf("Touch: X:%d, Y:%d\n", data->point.x, data->point.y);
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

uint32_t cntr = 0;
char str[128];

// Обработчик нажатия на кнопку
void action_increase_cntr(lv_event_t * e) {
  // if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
      Serial.println("Кнопка успешно нажата пальцем!");
      cntr++;
      sprintf(str, "Clicked %d", cntr);
      if(objects.btn_label) lv_label_set_text(objects.btn_label, str);

  // }
}

void action_to_settings(lv_event_t *e) {
  loadScreen(SCREEN_ID_SETTINGS);
}

void action_change_brightness(lv_event_t * e)
{
  // Получаем указатель на сам слайдер, который вызвал событие
  lv_obj_t * slider = (lv_obj_t*)lv_event_get_target(e);
  
  // Считываем его текущее числовое значение (int)
  int slider_value = lv_slider_get_value(slider);

  ledcWrite(GFX_BL, slider_value);
}

void setup() {
  Serial.begin(115200);

  platform_can_init();
  can_node_panel_bus0_init(0, 0, 0, &can_out, &can_in);

  Serial.println("\n=== ЗАПУСК ПЛАТЫ С ARDUINO_RGB_DISPLAY ===");

  // --- Инициализация тачскрина ---
  ts.begin();
  // Если нажатия будут инвертированы по осям, эти параметры можно изменить (setRotation)
  ts.setRotation(ROTATION_INVERTED);

  if (!gfx.begin(16000000)) { 
      Serial.println("❌ КРИТИЧЕСКАЯ ОШИБКА: RGB Panel не отвечает.");
      while (1) { delay(500); } 
  }

  gfx.fillScreen(0x0000); 

  // --- Инициализация ШИМ для подсветки ---
  ledcAttach(GFX_BL, PWM_FREQ, PWM_BITS);  // Привязка частоты и разрядности напрямую к GPIO
  ledcWrite(GFX_BL, 255);                  // Управление пином напрямую вместо указания канала (макс. яркость)

  lv_init();
  lv_tick_set_cb(millis);

  // Версия для динамического выделения буфера
  // disp_draw_buf = (uint8_t *)heap_caps_malloc(DISP_DRAW_BUF_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  // if (!disp_draw_buf) {
  //     Serial.println("❌ ОШИБКА: Нехватка внутренней памяти SRAM под буфер LVGL!");
  //     while(1) { delay(500); }
  // }
  
  // Регистрация экрана
  lv_display_t * display = lv_display_create(480, 480);
  lv_display_set_buffers(display, disp_draw_buf, NULL, DISP_DRAW_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(display, my_disp_flush);

  // --- Регистрация тачскрина в LVGL v9 ---
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  // // Создание интерфейса кнопки на экране через LVGL v9
  // lv_obj_t * btn = lv_button_create(lv_screen_active());
  // lv_obj_set_size(btn, 240, 80);
  // lv_obj_center(btn);
  // lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_ALL, NULL);

  // btn_label = lv_label_create(btn);
  // lv_label_set_text(btn_label, "Hello World");
  // lv_obj_center(btn_label);

  Serial.println("🎉 Система успешно запущена с тачскрином!");
  ui_init(); // Запуск графического интерфейса EEZ

  // Принудительно назначаем обработчик событий на наш экран
  // (Переменная ui_main_screen или objects.main_screen зависит от версии EEZ Studio)
  // lv_obj_add_event_cb(objects.settings, my_swipe_event_handler, LV_EVENT_GESTURE, NULL);
  time_stamp = millis();
}

void loop() {
  lv_timer_handler_run_in_period(5); 
  

  if(platform_can_poll(0))
  {
    //CAN ACTIVE
  }
  can_node_panel_bus0_rx(&can_in);
  can_node_panel_bus0_tx(&can_out);
  can_node_panel_bus0_update_timers((millis() - time_stamp) * 1300);
  time_stamp = millis();
}

// Функция для отправки сообщения
void sendCanMessage() {
  twai_message_t tx_msg = {};
  
  tx_msg.identifier = 0x123;           // ID сообщения (в формате HEX)
  tx_msg.extd = 0;                     // 0 = стандартный ID (11 бит), 1 = расширенный (29 бит)
  tx_msg.rtr = 0;                      // 0 = обычный кадр данных, 1 = запрос удаленной передачи (RTR)
  tx_msg.data_length_code = 8;         // Количество байт данных (от 0 до 8)
  
  // Заполняем массив данных (максимум 8 байт)
  tx_msg.data[0] = 0xAA;
  tx_msg.data[1] = 0xBB;
  tx_msg.data[2] = 0xCC;
  tx_msg.data[3] = 0xDD;

  // Отправка сообщения (таймаут ожидания свободной шины 100 мс)
  esp_err_t result = twai_transmit(&tx_msg, pdMS_TO_TICKS(0));
  
  if (result == ESP_OK) {
    Serial.println("Сообщение успешно отправлено в шину!");
  } else {
    Serial.print("Ошибка отправки! Код ошибки: ");
    Serial.println(result);
  }
}

// === ОБРАБОТЧИК СВАЙПОВ ===
void action_to_mian(lv_event_t *e) {
  // Получаем направление свайпа
  lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
  loadScreen(SCREEN_ID_MAIN);
  switch(dir) {
    case LV_DIR_LEFT:
      loadScreen(SCREEN_ID_MAIN);
      break;
      
    case LV_DIR_RIGHT:
      loadScreen(SCREEN_ID_MAIN);
      break;
      
    case LV_DIR_BOTTOM:
      Serial.println("Свайп сверху вниз");
      break;
      
    case LV_DIR_TOP:
      Serial.println("Свайп снизу вверх");
      break;
  }
}
