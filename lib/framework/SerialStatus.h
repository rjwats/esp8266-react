#ifndef SerialStatus_h
#define SerialStatus_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <SerialSettingsService.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>

#define MAX_SERIAL_STATUS_SIZE 1024
#define SERIAL_STATUS_SERVICE_PATH "/rest/serialStatus"

class SerialStatus {
 public:
  SerialStatus(AsyncWebServer* server, SerialSettingsService* serialSettingsService, SecurityManager* securityManager);

 private:
  SerialSettingsService* _serialSettingsService;

  void serialStatus(AsyncWebServerRequest* request);
};

#endif  // end SerialStatus_h
