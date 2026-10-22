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















// --- УПРАВЛЕНИЕ ПОДСВЕТКОЙ через PWM (LEDC) ---
#define GFX_BL 38      // Пин, подключенный к подсветке
#define PWM_FREQ 1000  // Частота ШИМ-сигнала (1 кГц)
#define PWM_BITS 8


// Размер буфера для экрана
#define DISP_DRAW_BUF_SIZE (2 * 480 * 40)
uint8_t disp_draw_buf[DISP_DRAW_BUF_SIZE] = {};


// Функция отрисовки фрейма
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  // gfx.draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);


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

        // std::vector<TouchButton> buttons;
        // i = 0;
        // touch->getButtons(buttons);
        // for (auto &button : buttons) {
        //     Serial.printf("Touch button(%d): %d\n", i++, button.second);
        // }

        if (!touch->isInterruptEnabled()) {
            delay(MY_TOUCH_READ_PERIOD_MS);
        }
    }

  // ts.read(); // Опрашиваем чип GT911

  // if (ts.isTouched) {
  //   data->state = LV_INDEV_STATE_PRESSED;
    
  //   // Получаем координаты первой точки касания
  //   data->point.x = ts.points[0].x;
  //   data->point.y = ts.points[0].y;
    
  //   // Опционально: вывод координат в консоль для проверки зеркальности
  //   // Serial.printf("Touch: X:%d, Y:%d\n", data->point.x, data->point.y);
  // } else {
  //   data->state = LV_INDEV_STATE_RELEASED;
  // }
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

    Serial.println("Testing LCD");
    auto lcd = board->getLCD();
    if (lcd != nullptr) {
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
        Serial.println("Draw color bar from top to bottom, the order is B - G - R");
        lcd->colorBarTest();
    } else {
        Serial.println("LCD is not available");
    }

    if (backlight != nullptr) {
        Serial.println("Turn on the backlight");
        backlight->on();
    }

    Serial.println("Testing Touch");
    auto touch = board->getTouch();
    if (touch != nullptr) {
#if MY_TOUCH_ENABLE_INTERRUPT_CALLBACK
        if (touch->isInterruptEnabled()) {
            touch->attachInterruptCallback(onTouchInterruptCallback, nullptr);
        }
#endif
        Serial.println("Reading touch points...");
    } else {
        Serial.println("Touch is not available");
    }

  // --- Инициализация тачскрина ---
  // ts.begin();

  // touch = create_touch_without_config();
  //   auto bus = static_cast<esp_panel::drivers::BusI2C *>(touch->getBus());
  //   bus->configI2C_FreqHz(400*1000);
  //   bus->configI2C_PullupEnable(1, 1);
  //   touch->configResetActiveLevel(1);

  // assert(touch->begin());
  // if (touch->isInterruptEnabled()) {
  //       touch->attachInterruptCallback(onTouchInterruptCallback);
  //   }
  
  // Wire.setTimeOut(50);
  // Если нажатия будут инвертированы по осям, эти параметры можно изменить (setRotation)
  // ts.setRotation(ROTATION_INVERTED);

  // if (!gfx.begin()) {
  //     Serial.println("❌ КРИТИЧЕСКАЯ ОШИБКА: RGB Panel не отвечает.");
  // }

  // gfx.fillScreen(0x0000); 

  // --- Инициализация ШИМ для подсветки ---
  // ledcAttach(GFX_BL, PWM_FREQ, PWM_BITS);  // Привязка частоты и разрядности напрямую к GPIO
  // ledcWrite(GFX_BL, 255);                  // Управление пином напрямую вместо указания канала (макс. яркость)
}

void set_brightness(int value)
{
  ledcWrite(GFX_BL, value);
}

#endif //SCREEN_H
