#ifndef ESP8266React_h
#define ESP8266React_h

#include <Arduino.h>

#ifdef ESP32
#include <AsyncTCP.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <APSettingsService.h>
#include <APStatus.h>
#include <AuthenticationService.h>
#include <FactoryResetService.h>
#include <MqttSettingsService.h>
#include <MqttStatus.h>
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

  StatefulService<SecuritySettings>* getSecuritySettingsService() {
    return &_securitySettingsService;
  }

  StatefulService<WiFiSettings>* getWiFiSettingsService() {
    return &_wifiSettingsService;
  }

  StatefulService<APSettings>* getAPSettingsService() {
    return &_apSettingsService;
  }

  StatefulService<NTPSettings>* getNTPSettingsService() {
    return &_ntpSettingsService;
  }

  StatefulService<OTASettings>* getOTASettingsService() {
    return &_otaSettingsService;
  }

  StatefulService<MqttSettings>* getMqttSettingsService() {
    return &_mqttSettingsService;
  }

  AsyncMqttClient* getMqttClient() {
    return _mqttSettingsService.getMqttClient();
  }

  void factoryReset() {
    _factoryResetService.factoryReset();
  }

 private:
  SecuritySettingsService _securitySettingsService;
  WiFiSettingsService _wifiSettingsService;
  APSettingsService _apSettingsService;
  NTPSettingsService _ntpSettingsService;
  OTASettingsService _otaSettingsService;
  MqttSettingsService _mqttSettingsService;
  RestartService _restartService;
  FactoryResetService _factoryResetService;

  AuthenticationService _authenticationService;

  WiFiScanner _wifiScanner;
  WiFiStatus _wifiStatus;
  NTPStatus _ntpStatus;
  APStatus _apStatus;
  MqttStatus _mqttStatus;
  SystemStatus _systemStatus;
};

#endif
