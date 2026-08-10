#include <DHT.h>

#define DHT22_PIN    2//40
#define DHTTYPE      DHT22

extern volatile float temperatureSelf;
extern volatile float humiditySelf;

TaskHandle_t dht22TaskHandle = NULL;

DHT dht(DHT22_PIN, DHTTYPE);

void dht22Task(void *pvParameters) {
  dht.begin();

  while (true) {
    // Чтение DHT22 занимает около 5 мс, библиотека сама считает пакет данных
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Проверяем корректность данных, чтобы не записать ошибку (NaN)
    if (!isnan(h) && !isnan(t)) {
      humiditySelf = h;
      temperatureSelf = t;
    }

    // DHT22 строго требует паузу минимум в 2 секунды между считываниями
    vTaskDelay(pdMS_TO_TICKS(2000)); 
  }
}

void tempSensorInit()
{
  // Создаем задачу FreeRTOS
  xTaskCreatePinnedToCore(
    dht22Task,            // Функция, которая будет выполняться
    "DHT22_Task",         // Имя задачи (для отладки)
    4096,                 // Размер стека в байтах
    NULL,                 // Параметры задачи
    1,                    // Приоритет задачи (1 - низкий, вполне достаточно)
    &dht22TaskHandle,     // Хэндл задачи
    0                     // На каком ядре запускать (Ядро 0)
  );
}