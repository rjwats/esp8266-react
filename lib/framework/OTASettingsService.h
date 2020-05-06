#ifndef OTASettingsService_h
#define OTASettingsService_h

#include <SettingsEndpoint.h>
#include <SettingsPersistence.h>

#ifdef ESP32
#include <ESPmDNS.h>
#elif defined(ESP8266)
#include <ESP8266mDNS.h>
#endif

#include <ArduinoOTA.h>
#include <WiFiUdp.h>

// Emergency defaults
#define DEFAULT_OTA_PORT 8266
#define DEFAULT_OTA_PASSWORD "esp-react"
#define DEFAULT_OTA_ENABLED true

#define OTA_SETTINGS_FILE "/config/otaSettings.json"
#define OTA_SETTINGS_SERVICE_PATH "/rest/otaSettings"

class OTASettings {
 public:
  bool enabled;
  int port;
  String password;

  static void serialize(OTASettings& settings, JsonObject& root) {
    root["enabled"] = settings.enabled;
    root["port"] = settings.port;
    root["password"] = settings.password;
  }

  static void deserialize(JsonObject& root, OTASettings& settings) {
    settings.enabled = root["enabled"] | DEFAULT_OTA_ENABLED;
    settings.port = root["port"] | DEFAULT_OTA_PORT;
    settings.password = root["password"] | DEFAULT_OTA_PASSWORD;
  }
};

class OTASettingsService : public SettingsService<OTASettings> {
 public:
  OTASettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();
  void loop();

 private:
  SettingsEndpoint<OTASettings> _settingsEndpoint;
  SettingsPersistence<OTASettings> _settingsPersistence;
  ArduinoOTAClass* _arduinoOTA;

  void configureArduinoOTA();
#ifdef ESP32
  void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ESP8266)
  WiFiEventHandler _onStationModeGotIPHandler;
  void onStationModeGotIP(const WiFiEventStationModeGotIP& event);
#endif
};

#endif  // end OTASettingsService_h
