#ifndef Ser2netStatus_h
#define Ser2netStatus_h

#include <time.h>
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

#define MAX_SER2NET_STATUS_SIZE 1024
#define SER2NET_STATUS_SERVICE_PATH "/rest/ser2netStatus"

class Ser2netStatus {
 public:
  Ser2netStatus(AsyncWebServer* server, SecurityManager* securityManager);

 private:
  void ser2netStatus(AsyncWebServerRequest* request);
};

#endif  // end Ser2netStatus_h
