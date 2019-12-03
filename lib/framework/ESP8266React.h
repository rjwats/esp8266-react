#ifndef ESP8266React_h
#define ESP8266React_h

#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <WiFi.h>
#endif

#include <APSettingsService.h>
#include <APStatus.h>
#include <AuthenticationService.h>
#include <FS.h>
#include <NTPSettingsService.h>
#include <NTPStatus.h>
#include <OTASettingsService.h>
#include <RestartService.h>
#include <SecuritySettingsService.h>
#include <SystemStatus.h>
#include <WiFiScanner.h>
#include <WiFiSettingsService.h>
#include <WiFiStatus.h>

class ESP8266React {
 public:
  ESP8266React(AsyncWebServer* server, FS* fs);

  void begin();
  void loop();

  SecurityManager* getSecurityManager() {
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
