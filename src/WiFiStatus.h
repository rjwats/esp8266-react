#ifndef WiFiStatus_h
#define WiFiStatus_h

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <IPAddress.h>

#define WIFI_STATUS_SERVICE_PATH "/rest/wifiStatus"

class WiFiStatus {

  public:

    WiFiStatus(AsyncWebServer *server);

  private:

    AsyncWebServer* _server;

    // handler refrences for logging important WiFi events over serial
    WiFiEventHandler _onStationModeConnectedHandler;
    WiFiEventHandler _onStationModeDisconnectedHandler;
    WiFiEventHandler _onStationModeGotIPHandler;

    // static functions for logging WiFi events to the UART
    static void onStationModeConnected(const WiFiEventStationModeConnected& event);
    static void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event);
    static void onStationModeGotIP(const WiFiEventStationModeGotIP& event);

    void wifiStatus(AsyncWebServerRequest *request);

};

#endif // end WiFiStatus_h
