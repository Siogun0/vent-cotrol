#include <OneWire.h>
#include <DallasTemperature.h>

// ===================================================================================
// !!! БИБЛИОТЕКА ТРЕБУЕТ ДОРАБОТКИ !!!
// 
// Необходимо убрать проверку номера пина больше 33
// if ( digitalPinIsValid(pin) && pin <= 33 ) // pins above 33 can be only inputs
//                            ^^^^^^^^^^^^^
// ===================================================================================

// Указываем пин подключения DATA
#define ONE_WIRE_BUS 40

extern volatile float temperatureSelf;

// Настраиваем объект OneWire для работы по этому пину
OneWire oneWire(ONE_WIRE_BUS);

// Передаем ссылку на OneWire в библиотеку DallasTemperature
DallasTemperature sensors(&oneWire);

// Хэндл задачи (нужен для управления)
TaskHandle_t ds18b20TaskHandle = NULL;

// Функция-задача для FreeRTOS
void ds18b20Task(void *pvParameters)
{
  sensors.begin();
  // Отключаем блокировку, так как мы сами контролируем паузы
  sensors.setWaitForConversion(false); 

  while (true) {
    // 1. Запрашиваем температуру
    sensors.requestTemperatures();
    // 2. Отправляем задачу "спать" на 750 мс, пока датчик считает данные.
    // В это время процессор полностью переключается на другие задачи!
    vTaskDelay(pdMS_TO_TICKS(750));
    
    // 3. Читаем результат в глобальную переменную
    float temp = sensors.getTempCByIndex(0);
    // Защита памяти (атомарное обновление для простых типов данных)
    temperatureSelf = temp;

    // 4. Пауза между измерениями (например, 2 секунды)
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void tempSensorInit()
{
  // Создаем задачу FreeRTOS
  xTaskCreatePinnedToCore(
    ds18b20Task,          // Функция, которая будет выполняться
    "DS18B20_Task",       // Имя задачи (для отладки)
    4096,                 // Размер стека в байтах
    NULL,                 // Параметры задачи
    1,                    // Приоритет задачи (1 - низкий, вполне достаточно)
    &ds18b20TaskHandle,   // Хэндл задачи
    0                     // На каком ядре запускать (Ядро 0)
  );
}
