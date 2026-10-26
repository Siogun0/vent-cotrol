#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <esp_display_panel.hpp>

using namespace esp_panel::board;
using namespace esp_panel::drivers;
TaskHandle_t touchTaskHandle = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Please update the following configuration according to your test ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MY_LCD_ENABLE_PRINT_FPS            (0)
#define MY_LCD_ENABLE_DRAW_FINISH_CALLBACK (0)
#define MY_TOUCH_ENABLE_INTERRUPT_CALLBACK (0)
#define MY_TOUCH_ENABLE_READ_IN_TASK       (0)
#define MY_TOUCH_POINTS_NUMBER             (1)
#define MY_TOUCH_PERIOD                    (33)

Board* board = nullptr;
Touch* touch = nullptr;

#if MY_LCD_ENABLE_PRINT_FPS
#define MY_LCD_PRINT_FPS_PERIOD_MS         (1000)
#define MY_LCD_PRINT_FPS_COUNT_MAX         (50)

DRAM_ATTR int frame_count = 0;
DRAM_ATTR int fps = 0;
DRAM_ATTR long start_time = 0;

IRAM_ATTR bool onLCD_RefreshFinishCallback(void *user_data)
{
    if (start_time == 0) {
        start_time = millis();

        return false;
    }

    frame_count++;
    if (frame_count >= MY_LCD_PRINT_FPS_COUNT_MAX) {
        fps = MY_LCD_PRINT_FPS_COUNT_MAX * 1000 / (millis() - start_time);
        esp_rom_printf("LCD FPS: %d\n", fps);
        frame_count = 0;
        start_time = millis();
    }

    return false;
}
#endif // MY_LCD_ENABLE_PRINT_FPS

#if MY_LCD_ENABLE_DRAW_FINISH_CALLBACK
IRAM_ATTR bool onLCD_DrawFinishCallback(void *user_data)
{
    esp_rom_printf("LCD draw finish callback\n");

    return false;
}
#endif

#if MY_TOUCH_ENABLE_INTERRUPT_CALLBACK
IRAM_ATTR static bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif


// Размер буфера для экрана
#define DISP_DRAW_BUF_SIZE (2 * 480 * 40)
uint8_t disp_draw_buf[DISP_DRAW_BUF_SIZE] = {};


// Функция отрисовки фрейма
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;
  board->getLCD()->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)px_map);

  lv_display_flush_ready(disp);
}

// --- Функция чтения координат тачскрина для LVGL v9 ---
void my_touch_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  static int32_t last_x = 0;
  static int32_t last_y = 0;

#if !MY_TOUCH_ENABLE_READ_IN_TASK
  touch->readRawData(MY_TOUCH_POINTS_NUMBER, 0, 0);
#endif //!MY_TOUCH_ENABLE_READ_IN_TASK

  std::vector<TouchPoint> points;
  touch->getPoints(points);
        // for (auto &point : points) {
        //   Serial.printf("Touch point(%d): x %d, y %d, strength %d\n", i++, point.x, point.y, point.strength);
        // }
  if (points.size() > 0)
  {
    data->state = LV_INDEV_STATE_PRESSED;
    // Получаем координаты первой точки касания
    last_x = points[0].x;
    last_y = points[0].y;
  }
  else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
  data->point.x = last_x;
  data->point.y = last_y;
}

void touchPollTask(void *pvParameters) {
  while (true) {
    touch->readRawData(MY_TOUCH_POINTS_NUMBER, 0, 0);
    vTaskDelay(pdMS_TO_TICKS(MY_TOUCH_PERIOD));
  }
}

void set_brightness(int value)
{
  auto backlight = board->getBacklight();
  backlight->setBrightness(value * 100 / 255);
}

void screen_init(void)
{
    // Statically load the default board configuration
    board = new Board();
    assert(board->begin());

    auto backlight = board->getBacklight();
    if (backlight == nullptr) {
        set_brightness(100);
    } else {
        Serial.println("Backlight is not available");
    }

#if MY_LCD_ENABLE_PRINT_FPS
        // Only RGB and MIPI-DSI bus types support refresh finish callback
        auto bus_type = lcd->getBus()->getBasicAttributes().type;
        if ((bus_type == ESP_PANEL_BUS_TYPE_RGB) || (bus_type == ESP_PANEL_BUS_TYPE_MIPI_DSI)) {
            lcd->attachRefreshFinishCallback(onLCD_RefreshFinishCallback);
        }
#endif //MY_LCD_ENABLE_PRINT_FPS
#if MY_LCD_ENABLE_DRAW_FINISH_CALLBACK
        lcd->attachDrawBitmapFinishCallback(onLCD_DrawFinishCallback);
#endif //MY_LCD_ENABLE_DRAW_FINISH_CALLBACK

    touch = board->getTouch();
    if (touch != nullptr) {
#if MY_TOUCH_ENABLE_READ_IN_TASK
        xTaskCreatePinnedToCore(
            touchPollTask,        // Функция, которая будет выполняться
            "Touch_Task",         // Имя задачи (для отладки)
            4096,                 // Размер стека в байтах
            NULL,                 // Параметры задачи
            1,                    // Приоритет задачи (1 - низкий, вполне достаточно)
            &touchTaskHandle,     // Хэндл задачи
            1                     // На каком ядре запускать (Ядро 0)
        );
#endif //MY_TOUCH_ENABLE_READ_IN_TASK
#if MY_TOUCH_ENABLE_INTERRUPT_CALLBACK        
        if (touch->isInterruptEnabled()) {
            touch->attachInterruptCallback(onTouchInterruptCallback, nullptr);
        }
#endif //MY_TOUCH_ENABLE_INTERRUPT_CALLBACK
    }
    else {
      Serial.println("Touch not init!");
    }
}

#endif //SCREEN_H
