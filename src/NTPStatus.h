#ifndef NTPStatus_h
#define NTPStatus_h

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
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <SecurityManager.h>

#define MAX_NTP_STATUS_SIZE 1024
#define NTP_STATUS_SERVICE_PATH "/rest/ntpStatus"

class NTPStatus {

  public:

    NTPStatus(AsyncWebServer *server, SecurityManager* securityManager);

  private:

    AsyncWebServer* _server;
    SecurityManager* _securityManager;

    void ntpStatus(AsyncWebServerRequest *request);

};

#endif // end NTPStatus_h
