#ifndef WiFiStatus_h
#define WiFiStatus_h

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
#include <IPAddress.h>
#include <SecurityManager.h>

#define MAX_WIFI_STATUS_SIZE 1024
#define WIFI_STATUS_SERVICE_PATH "/rest/wifiStatus"

class WiFiStatus {

  public:

    WiFiStatus(AsyncWebServer *server, SecurityManager* securityManager);

  private:

    AsyncWebServer* _server;
    SecurityManager* _securityManager;

#if defined(ESP8266)
    // handler refrences for logging important WiFi events over serial
    WiFiEventHandler _onStationModeConnectedHandler;
    WiFiEventHandler _onStationModeDisconnectedHandler;
    WiFiEventHandler _onStationModeGotIPHandler;    
    // static functions for logging WiFi events to the UART
    static void onStationModeConnected(const WiFiEventStationModeConnected& event);
    static void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event);
    static void onStationModeGotIP(const WiFiEventStationModeGotIP& event);
#elif defined(ESP_PLATFORM)
    // static functions for logging WiFi events to the UART
    static void onStationModeConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);    
#endif

    void wifiStatus(AsyncWebServerRequest *request);

};

#endif // end WiFiStatus_h
