#ifndef SystemStatus_h
#define SystemStatus_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncArduinoJson6.h>
#include <SecurityManager.h>

#define MAX_ESP_STATUS_SIZE 1024
#define SYSTEM_STATUS_SERVICE_PATH "/rest/systemStatus"

class SystemStatus {

  public:

    SystemStatus(AsyncWebServer *server, SecurityManager* securityManager);

  private:

    AsyncWebServer* _server;
    SecurityManager* _securityManager;

    void systemStatus(AsyncWebServerRequest *request);

};

#endif // end SystemStatus_h