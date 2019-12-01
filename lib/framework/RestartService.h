#ifndef RestartService_h
#define RestartService_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>

#define RESTART_SERVICE_PATH "/rest/restart"

class RestartService {

  public:
   
    RestartService(AsyncWebServer* server, SecurityManager* securityManager);

  private:

    void restart(AsyncWebServerRequest *request);

};

#endif // end RestartService_h