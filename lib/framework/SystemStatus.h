#ifndef SystemStatus_h
#define SystemStatus_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
#include <ESPFS.h>

#define MAX_ESP_STATUS_SIZE 1024
#define SYSTEM_STATUS_SERVICE_PATH "/rest/systemStatus"

class SystemStatus {
 public:
  SystemStatus(AsyncWebServer* server, SecurityManager* securityManager);

 private:
  void systemStatus(AsyncWebServerRequest* request);
};

#endif  // end SystemStatus_h
