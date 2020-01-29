#ifndef ESP8266React_h
#define ESP8266React_h

#include <Arduino.h>

#ifdef ESP32
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <FS.h>
#endif

#include <APSettingsService.h>
#include <APStatus.h>
#include <AuthenticationService.h>
#include <NTPSettingsService.h>
#include <NTPStatus.h>
#include <OTASettingsService.h>
#include <RestartService.h>
#include <SecuritySettingsService.h>
#include <SystemStatus.h>
#include <WiFiScanner.h>
#include <WiFiSettingsService.h>
#include <WiFiStatus.h>

#ifdef PROGMEM_WWW
#include <WWWData.h>
#endif

class ESP8266React {
 public:
  ESP8266React(AsyncWebServer* server, FS* fs);

  void begin();
  void loop();

  SecurityManager* getSecurityManager() {
    return &_securitySettingsService;
  }

   SettingsService<SecuritySettings>* getSecuritySettingsService() {
    return &_securitySettingsService;
  }

  SettingsService<WiFiSettings>* getWiFiSettingsService() {
    return &_wifiSettingsService;
  }

  SettingsService<APSettings>* getAPSettingsService() {
    return &_apSettingsService;
  }

  SettingsService<NTPSettings>* getNTPSettingsService() {
    return &_ntpSettingsService;
  }

  SettingsService<OTASettings>* getOTASettingsService() {
    return &_otaSettingsService;
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
