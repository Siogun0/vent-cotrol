#include "server.h"

// extern fs::LittleFSFS LittleFS;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events)


String generateTuyaJson() {
    // Выделяем память под JSON-документ (StaticJsonDocument для ArduinoJson v6, JsonDocument для v7)
    // 1024 байт с запасом хватит для данной структуры
    StaticJsonDocument<1024> doc; 

    // Заполняем корень документа
    doc["time"] = String(get_var_time_str());

    // Вложенный объект "sensors"
    JsonObject jsonSensors = doc.createNestedObject("sensors");
    jsonSensors["temp"] = get_var_temperature();
    jsonSensors["humidity"] = get_var_humidity();
    jsonSensors["voltage"] = get_var_voltage();
    jsonSensors["current"] = get_var_current();
    jsonSensors["power"] = get_var_power();

    // Вложенный объект "network"
    JsonObject jsonNetwork = doc.createNestedObject("network");
    jsonNetwork["bt"]   = (int)get_var_bt_status();
    jsonNetwork["wifi"] = (int)get_var_wifi_status();
    jsonNetwork["can"]  = (int)get_var_can_status();

    // Вложенный массив устройств "devices"
    JsonArray jsonDevices = doc.createNestedArray("devices");

    //Системные параметры и сценарии (dev_system)
    JsonObject devSystem = jsonDevices.createNestedObject();
    devSystem["devId"] = "dev_system";
    JsonObject dpsSystem = devSystem.createNestedObject("dps");
    dpsSystem["10"] = (int32_t)get_var_scenario_id_active(); // Запись сценария в DP 10

    // Устройство 1: Вентиляторы (dev_fan_01)
    JsonObject dev1 = jsonDevices.createNestedObject();
    dev1["devId"] = "dev_fan_01";
    JsonObject dps1 = dev1.createNestedObject("dps");
    dps1["1"] = get_var_supply_fan_speed();
    dps1["2"] = get_var_exhaust_fan_speed();

    // Устройство 2: Приточные клапана (dev_valves_zone1)
    JsonObject dev2 = jsonDevices.createNestedObject();
    dev2["devId"] = "dev_valves_zone1";
    JsonObject dps2 = dev2.createNestedObject("dps");
    dps2["101"] = get_var_family_room_valve();
    dps2["102"] = get_var_master_badroom_valve();
    dps2["103"] = get_var_children_room_valve();
    dps2["104"] = get_var_pantry_room_valve();

    // Устройство 3: Вытяжные клапана (dev_valves_zone2)
    JsonObject dev3 = jsonDevices.createNestedObject();
    dev3["devId"] = "dev_valves_zone2";
    JsonObject dps3 = dev3.createNestedObject("dps");
    dps3["201"] = get_var_toilet_exhaust_valve();
    dps3["202"] = get_var_bathroom_exhaust_valve();

    // Сериализуем данные в C++ строку String
    String output;
    
    // Если вам нужен красивый форматированный вывод с отступами (как в вашем примере):
    serializeJsonPretty(doc, output);
    
    // Если нужна компактная строка без пробелов для быстрой передачи по сети:
    // serializeJson(doc, output);

    return output;
}

bool parseTuyaCommand(const String& jsonInput) {
    // Выделяем память под входящий JSON (512 байт для одной команды более чем достаточно)
    StaticJsonDocument<512> doc;

    // Десериализуем (парсим) строку
    DeserializationError error = deserializeJson(doc, jsonInput);

    // Если JSON некорректен, выходим с ошибкой
    if (error) {
        Serial.print(F("Ошибка парсинга JSON: "));
        Serial.println(error.f_str());
        return false;
    }

    // Проверяем наличие обязательных полей
    if (!doc.containsKey("devId") || !doc.containsKey("dps")) {
        Serial.println(F("Ошибка: В JSON отсутствуют поля devId или dps"));
        return false;
    }

    const char* devId = doc["devId"];
    JsonObject dps = doc["dps"];

    // Флаг, определяющий, нашли ли мы нужное устройство и точку данных
    bool updateSuccess = false;

    // 0. Обработка системного устройства сценариев
    if (strcmp(devId, "dev_system") == 0) {
        if (dps.containsKey("10")) {
            int32_t newScenario = dps["10"].as<int32_t>();
            if (newScenario >= 0 && newScenario <= 8) {
                set_var_scenario_id_active(newScenario);
                Serial.printf("Tuya CMD: Системный сценарий изменен на %d\n", newScenario);
            }
        }
    }

    // 1. Проверяем устройство: ВЕНТИЛЯТОРЫ
    if (strcmp(devId, "dev_fan_01") == 0) {
        if (dps.containsKey("1")) {
            set_var_supply_fan_speed(dps["1"].as<int>());
            // Serial.println(F("Обновлено: Приток -> "));
            updateSuccess = true;
        }
        if (dps.containsKey("2")) {
            set_var_exhaust_fan_speed(dps["2"].as<int>());
            // Serial.println(F("Обновлено: Вытяжка -> "));
            updateSuccess = true;
        }
    }
    // 2. Проверяем устройство: ПРИТОЧНЫЕ КЛАПАНА
    else if (strcmp(devId, "dev_valves_zone1") == 0) {
        if (dps.containsKey("101")) {
            set_var_family_room_valve(dps["101"].as<int>());
            // Serial.println(F("Обновлено: Клапан Гостиная -> "));
            updateSuccess = true;
        }
        if (dps.containsKey("102")) {
            set_var_master_badroom_valve(dps["102"].as<int>());
            // Serial.println(F("Обновлено: Клапан Спальня -> "));
            updateSuccess = true;
        }
        if (dps.containsKey("103")) {
            set_var_children_room_valve(dps["103"].as<int>());
            // Serial.println(F("Обновлено: Клапан Детская -> "));
            updateSuccess = true;
        }
        if (dps.containsKey("104")) {
            set_var_pantry_room_valve(dps["104"].as<int>());
            // Serial.println(F("Обновлено: Клапан Кладовка -> "));
            updateSuccess = true;
        }
    }
    // 3. Проверяем устройство: ВЫТЯЖНЫЕ КЛАПАНА
    else if (strcmp(devId, "dev_valves_zone2") == 0) {
        if (dps.containsKey("201")) {
            set_var_toilet_exhaust_valve(dps["201"].as<int>());
            // Serial.println(F("Обновлено: Клапан Туалет -> "));
            updateSuccess = true;
        }
        if (dps.containsKey("202")) {
            set_var_bathroom_exhaust_valve(dps["202"].as<int>());
            // Serial.println(F("Обновлено: Клапан Ванная -> "));
            updateSuccess = true;
        }
    }

    if (!updateSuccess) {
        Serial.print(F("Предупреждение: Неизвестный devId или dpsId: "));
        Serial.println(devId);
    }

    return updateSuccess;
}

void serverRun(fs::FS &fs)
{
  // Сервер HTTP
  server.serveStatic("/", fs, "/").setDefaultFile("manual.html");
  // respond to GET requests on URL /heap
  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });
  server.on("/api/tuya/status", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", generateTuyaJson());
  });
  server.on("/api/tuya/control", HTTP_POST,
    [](AsyncWebServerRequest *request) {
        // Этот пустой обработчик нужен по синтаксису библиотеки,
        // основной ответ мы отправим чуть ниже в блоке onBody
    },
    NULL, // upload handler (не используется)
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {

        // 1. Собираем входящие байты данных в строку C++ String
        String jsonBuffer = "";
        for (size_t i = 0; i < len; i++) {
            jsonBuffer += (char)data[i];
        }

        // 2. Вызываем наш парсер и обновляем глобальные переменные
        bool isOk = parseTuyaCommand(jsonBuffer);

        // 3. Отвечаем клиенту (фронтенду) в зависимости от результата парсинга
        if (isOk) {
            // Успех — отправляем JSON с подтверждением
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            // Ошибка парсинга или неизвестное устройство
            request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Bad Request or Device Not Found\"}");
        }
    }
  );
  server.begin();
  Serial.println("WEB сервер запущен");
}