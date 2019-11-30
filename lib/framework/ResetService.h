#ifndef ResetService_h
#define ResetService_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>

#define RESET_SERVICE_PATH "/rest/reset"

class ResetService {

  public:
   
    ResetService(AsyncWebServer* server, SecurityManager* securityManager);

  private:

    void reset(AsyncWebServerRequest *request);

};

#endif // end ResetService_h