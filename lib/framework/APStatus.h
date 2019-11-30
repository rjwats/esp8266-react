#ifndef APStatus_h
#define APStatus_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <IPAddress.h>
#include <SecurityManager.h>

#define MAX_AP_STATUS_SIZE 1024
#define AP_STATUS_SERVICE_PATH "/rest/apStatus"

class APStatus {

  public:

    APStatus(AsyncWebServer* server, SecurityManager* securityManager);

  private:
  
    void apStatus(AsyncWebServerRequest *request);

};

#endif // end APStatus_h
