#ifndef ESP8266React_h
#define ESP8266React_h

#include <Arduino.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
#endif

#include <FS.h>
#include <SecuritySettingsService.h>
#include <WiFiSettingsService.h>
#include <APSettingsService.h>
#include <NTPSettingsService.h>
#include <OTASettingsService.h>
#include <AuthenticationService.h>
#include <WiFiScanner.h>
#include <WiFiStatus.h>
#include <NTPStatus.h>
#include <APStatus.h>
#include <SystemStatus.h>
#include <RestartService.h>

class ESP8266React {

  public:

    ESP8266React(AsyncWebServer* server, FS* fs);
    
    void begin();
    void loop();
    
    SecurityManager* getSecurityManager(){
        return &_securitySettingsService;
    }

  private:

    SecuritySettingsService _securitySettingsService;

    WiFiSettingsService _wifiSettingsService;
    APSettingsService _apSettingsService;
    NTPSettingsService _ntpSettingsService;
    OTASettingsService _otaSettingsService;
    RestartService _restartService;    
    AuthenticationService _authenticationService;

    
    WiFiScanner _wifiScanner;
    WiFiStatus _wifiStatus;
    NTPStatus _ntpStatus; 
    APStatus _apStatus;
    SystemStatus _systemStatus;

};

#endif
