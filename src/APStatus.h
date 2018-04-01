#ifndef APStatus_h
#define APStatus_h

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <IPAddress.h>

#define AP_STATUS_SERVICE_PATH "/rest/apStatus"

class APStatus {

  public:

    APStatus(AsyncWebServer *server);

  private:

    AsyncWebServer* _server;

    void apStatus(AsyncWebServerRequest *request);

};

#endif // end APStatus_h
