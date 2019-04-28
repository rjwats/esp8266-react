#ifndef ESPStatus_h
#define ESPStatus_h

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

#define MAX_ESP_STATUS_SIZE 1024
#define ESP_STATUS_SERVICE_PATH "/rest/espStatus"

class ESPStatus {

  public:

    ESPStatus(AsyncWebServer *server);

  private:

    AsyncWebServer* _server;

    void espStatus(AsyncWebServerRequest *request);

};

#endif // end ESPStatus_h
