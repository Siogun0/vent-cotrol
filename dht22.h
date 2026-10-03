#include "dhtESP32-rmt.h"

#define DHT22_PIN    40
#define DHTTYPE      DHT22

extern volatile float temperatureSelf;
extern volatile float humiditySelf;

TaskHandle_t dht22TaskHandle = NULL;

void dht22Task(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(2000));
  while (true) {
    float h;
    float t;

    uint8_t error = read_dht(t, h, DHT22_PIN, DHTTYPE);
    if (!error) {
      humiditySelf = h;
      temperatureSelf = t;
      // Serial.printf("%f C %f % \n", t, h);
    }
    else
    {
      Serial.printf("Error reading DHT22: %d\n", error);
    }

    vTaskDelay(pdMS_TO_TICKS(10000));
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

/*
Error codes
0: OK
1: TOO_SOON
2: DRIVER
3: TIMEOUT
4: NACK
5: BAD_DATA
6: CHECKSUM
7: UNDERFLOW
8: OVERFLOW
*/