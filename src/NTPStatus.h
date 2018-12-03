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
#include <AsyncJson.h>
#include <TimeLib.h>
#include <NtpClientLib.h>

#define NTP_STATUS_SERVICE_PATH "/rest/ntpStatus"

class NTPStatus {

  public:

    NTPStatus(AsyncWebServer *server);

  private:

    AsyncWebServer* _server;

    void ntpStatus(AsyncWebServerRequest *request);

};

#endif // end NTPStatus_h
