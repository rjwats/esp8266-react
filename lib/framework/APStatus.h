#ifndef APStatus_h
#define APStatus_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <APSettingsService.h>
#include <ESPAsyncWebServer.h>
#include <IPAddress.h>
#include <SecurityManager.h>

#define MAX_AP_STATUS_SIZE 1024
#define AP_STATUS_SERVICE_PATH "/rest/apStatus"

class APStatus {
  APSettingsService* apSettingsService;

  enum Status {
    ACTIVE = 1,
    INACTIVE = 2,
    WAITING_CLIENTS_DISCONNECT = 3,
  };

 public:
  APStatus(APSettingsService* apSettingsService, AsyncWebServer* server, SecurityManager* securityManager);

 private:
  void apStatus(AsyncWebServerRequest* request);
};

#endif  // end APStatus_h
