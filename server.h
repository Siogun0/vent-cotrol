#ifndef SERVER_H
#define SERVER_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "native_variables.h"

void serverRun(fs::FS &fs);

#endif //SERVER_H