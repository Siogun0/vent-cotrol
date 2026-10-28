#ifndef SENSOR_REQUEST_H
#define SENSOR_REQUEST_H

#include <AsyncTCP.h>
#include <ESPAsyncWebClient.h>
#include <ArduinoJson.h>
#include "native_variables.h"
#include <LD2410Types.h>

AsyncHttpClient client;
TaskHandle_t sensor_request_Handle = NULL;

// Внутренняя структура для хранения данных радара
// typedef LD2410Types::DetectionData RadarData;
struct RadarData : public LD2410Types::DetectionData
{};

RadarData sensor_bedroom;

void deserializeRadarData(const JsonDocument& doc, RadarData& data)
{
  // Системные данные
  data.timestamp = doc["timestamp_ms"] | 0UL;
  data.engineeringMode = doc["engineering_mode"] | false;

  // Базовые результаты детекции
  data.presenceDetected = doc["presence_detected"] | false;
  data.movingPresenceDetected = doc["moving_presence_detected"] | false;
  data.stationaryPresenceDetected = doc["stationary_presence_detected"] | false;
  
  // Приведение типа из int к вашему Enum типу TargetState
  int targetStateInt = doc["target_state"] | 0;
  data.targetState = static_cast<LD2410Types::TargetState>(targetStateInt);

  data.movingTargetDistance = doc["moving_target_distance_cm"] | 0;
  data.movingTargetSignal = doc["moving_target_signal"] | 0;
  data.stationaryTargetDistance = doc["stationary_target_distance_cm"] | 0;
  data.stationaryTargetSignal = doc["stationary_target_signal"] | 0;
  data.detectedDistance = doc["detected_distance_cm"] | 0;

  // Дополнительные данные датчика
  data.lightLevel = doc["light_level"] | 0;
  data.outPinStatus = doc["out_pin_status"] | 0;

  // Данные инженерного режима (движущиеся цели)
  data.movingTargetGateSignalCount = doc["moving_gate_signal_count"] | 0;
  JsonArrayConst movingSignals = doc["moving_gate_signals"];
  if (movingSignals) {
    for (int i = 0; i < 9; i++) {
      // Заполняем массив, если элемент есть в JSON, иначе пишем 0
      data.movingTargetGateSignals[i] = movingSignals[i] | 0;
    }
  }

  // Данные инженерного режима (неподвижные цели)
  data.stationaryTargetGateSignalCount = doc["stationary_gate_signal_count"] | 0;
  JsonArrayConst stationarySignals = doc["stationary_gate_signals"];
  if (stationarySignals) {
    for (int i = 0; i < 9; i++) {
      data.stationaryTargetGateSignals[i] = stationarySignals[i] | 0;
    }
  }
}

void sensor_request_Task(void* period)
{
  while(true)
  {
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS((uint32_t)period));
    if (WiFi.status() == WL_CONNECTED)
    {
      client.get("http://sensorbedroom.local/ld2410_get", 
        [](std::shared_ptr<AsyncHttpResponse> response) {
            // Serial.printf("Body: %s\n", response->getBody().c_str());
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, response->getBody());
            if (!error)
            {
              deserializeRadarData(doc, sensor_bedroom);

              Serial.printf("Presence detect: %s\n", targetStateToString(sensor_bedroom.targetState));
            }
            
        },
        [](HttpClientError error, const char* message) {
            Serial.printf("Error sensorbedroom: %s (%d)\n", httpClientErrorToString(error), (int)error);
        }
      );
    }
  }
}

void sensor_request_init(TickType_t period)
{
  // Создаем задачу FreeRTOS
  xTaskCreate(
    sensor_request_Task,          // Функция, которая будет выполняться
    "sensor_request__Task",       // Имя задачи (для отладки)
    4096,                         // Размер стека в байтах
    (void*)period,                // Параметры задачи
    1,                            // Приоритет задачи (1 - низкий, вполне достаточно)
    &sensor_request_Handle        // Хэндл задачи
  );
}

void sensor_request_pull()
{
  xTaskNotifyGive(sensor_request_Handle);
}


#endif //SENSOR_REQUEST_H