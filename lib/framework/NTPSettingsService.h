#ifndef NTPSettingsService_h
#define NTPSettingsService_h

#include <SettingsEndpoint.h>
#include <SettingsPersistence.h>

#include <time.h>
#ifdef ESP32
#include <lwip/apps/sntp.h>
#elif defined(ESP8266)
#include <sntp.h>
#endif

// default time zone
#define NTP_SETTINGS_SERVICE_DEFAULT_ENABLED true
#define NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_LABEL "Europe/London"
#define NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_FORMAT "GMT0BST,M3.5.0/1,M10.5.0"
#define NTP_SETTINGS_SERVICE_DEFAULT_SERVER "time.google.com"

#define NTP_SETTINGS_FILE "/config/ntpSettings.json"
#define NTP_SETTINGS_SERVICE_PATH "/rest/ntpSettings"

class NTPSettings {
 public:
  bool enabled;
  String tzLabel;
  String tzFormat;
  String server;

  static void serialize(NTPSettings& settings, JsonObject& root) {
    root["enabled"] = settings.enabled;
    root["server"] = settings.server;
    root["tz_label"] = settings.tzLabel;
    root["tz_format"] = settings.tzFormat;
  }

  static void deserialize(JsonObject& root, NTPSettings& settings) {
    settings.enabled = root["enabled"] | NTP_SETTINGS_SERVICE_DEFAULT_ENABLED;
    settings.server = root["server"] | NTP_SETTINGS_SERVICE_DEFAULT_SERVER;
    settings.tzLabel = root["tz_label"] | NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_LABEL;
    settings.tzFormat = root["tz_format"] | NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_FORMAT;
  }
};

class NTPSettingsService : public SettingsService<NTPSettings> {
 public:
  NTPSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();

 private:
  SettingsEndpoint<NTPSettings> _settingsEndpoint;
  SettingsPersistence<NTPSettings> _settingsPersistence;

#ifdef ESP32
  void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
  void onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ESP8266)
  WiFiEventHandler _onStationModeDisconnectedHandler;
  WiFiEventHandler _onStationModeGotIPHandler;

  void onStationModeGotIP(const WiFiEventStationModeGotIP& event);
  void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event);
#endif
  void configureNTP();
};

#endif  // end NTPSettingsService_h
