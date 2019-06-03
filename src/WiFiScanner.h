#ifndef WiFiScanner_h
#define WiFiScanner_h

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
#include <SecurityManager.h>

#define SCAN_NETWORKS_SERVICE_PATH "/rest/scanNetworks"
#define LIST_NETWORKS_SERVICE_PATH "/rest/listNetworks"

#define MAX_WIFI_SCANNER_SIZE 1024

class WiFiScanner {

  public:

    WiFiScanner(AsyncWebServer *server, SecurityManager* securityManager);

  private:

    AsyncWebServer* _server;

    void scanNetworks(AsyncWebServerRequest *request);
    void listNetworks(AsyncWebServerRequest *request);

#if defined(ESP8266)
    uint8_t convertEncryptionType(uint8_t encryptionType);
#endif

};

#endif // end WiFiScanner_h
