#ifndef SCREEN_H
#define SCREEN_H


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

void screen_init(void)
{
  // --- Инициализация тачскрина ---
  ts.begin();
  // Если нажатия будут инвертированы по осям, эти параметры можно изменить (setRotation)
  ts.setRotation(ROTATION_INVERTED);

  if (!gfx.begin(16000000)) { 
      Serial.println("❌ КРИТИЧЕСКАЯ ОШИБКА: RGB Panel не отвечает.");
  }

  gfx.fillScreen(0x0000); 

  // --- Инициализация ШИМ для подсветки ---
  ledcAttach(GFX_BL, PWM_FREQ, PWM_BITS);  // Привязка частоты и разрядности напрямую к GPIO
  ledcWrite(GFX_BL, 255);                  // Управление пином напрямую вместо указания канала (макс. яркость)
}

#endif //SCREEN_H
