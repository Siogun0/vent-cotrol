#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <esp_display_panel.hpp>

using namespace esp_panel::board;
using namespace esp_panel::drivers;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Please update the following configuration according to your test ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MY_LCD_ENABLE_PRINT_FPS            (0)
#define MY_LCD_ENABLE_DRAW_FINISH_CALLBACK (0)
#define MY_TOUCH_ENABLE_INTERRUPT_CALLBACK (0)
#define MY_TOUCH_READ_PERIOD_MS            (30)

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

Board *board = nullptr;

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
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    auto touch = board->getTouch();
    if (touch != nullptr) {
        // Read all touch points and buttons
        touch->readRawData(-1, -1, MY_TOUCH_READ_PERIOD_MS);

        std::vector<TouchPoint> points;
        int i = 0;
        touch->getPoints(points);
        for (auto &point : points) {
          Serial.printf("Touch point(%d): x %d, y %d, strength %d\n", i++, point.x, point.y, point.strength);
        }
        if (points.size() > 0)
        {
          data->state = LV_INDEV_STATE_PRESSED;
          // Получаем координаты первой точки касания
          data->point.x = points[0].x;
          data->point.y = points[0].y;
        }
        else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    }
}

void screen_init(void)
{
    // Statically load the default board configuration
    board = new Board();
    assert(board->begin());

    auto backlight = board->getBacklight();
    if (backlight != nullptr) {
        Serial.println("Turn off the backlight");
        backlight->off();
    } else {
        Serial.println("Backlight is not available");
    }

#if MY_LCD_ENABLE_PRINT_FPS
        // Only RGB and MIPI-DSI bus types support refresh finish callback
        auto bus_type = lcd->getBus()->getBasicAttributes().type;
        if ((bus_type == ESP_PANEL_BUS_TYPE_RGB) || (bus_type == ESP_PANEL_BUS_TYPE_MIPI_DSI)) {
            lcd->attachRefreshFinishCallback(onLCD_RefreshFinishCallback);
        }
#endif
#if MY_LCD_ENABLE_DRAW_FINISH_CALLBACK
        lcd->attachDrawBitmapFinishCallback(onLCD_DrawFinishCallback);
#endif

    if (backlight != nullptr) {
        Serial.println("Turn on the backlight");
        backlight->on();
    }

#if MY_TOUCH_ENABLE_INTERRUPT_CALLBACK
    auto touch = board->getTouch();
    if (touch != nullptr) {
        if (touch->isInterruptEnabled()) {
            touch->attachInterruptCallback(onTouchInterruptCallback, nullptr);
        }
        Serial.println("Reading touch points...");
    }
#endif    
}

void set_brightness(int value)
{
  auto backlight = board->getBacklight();
  backlight->setBrightness(value * 100 / 255);
}

#endif //SCREEN_H
