#ifndef NTPSettingsService_h
#define NTPSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>

#include <time.h>
#ifdef ESP32
#include <lwip/apps/sntp.h>
#elif defined(ESP8266)
#include <sntp.h>
#endif

// default time zone
#ifndef NTP_SETTINGS_SERVICE_FACTORY_ENABLED
  #define NTP_SETTINGS_SERVICE_FACTORY_ENABLED true
#endif

#ifndef NTP_SETTINGS_SERVICE_FACTORY_TIME_ZONE_LABEL
  #define NTP_SETTINGS_SERVICE_FACTORY_TIME_ZONE_LABEL "Europe/London"
#endif

#ifndef NTP_SETTINGS_SERVICE_FACTORY_TIME_ZONE_FORMAT
  #define NTP_SETTINGS_SERVICE_FACTORY_TIME_ZONE_FORMAT "GMT0BST,M3.5.0/1,M10.5.0"
#endif

#ifndef NTP_SETTINGS_SERVICE_FACTORY_SERVER
  #define NTP_SETTINGS_SERVICE_FACTORY_SERVER "time.google.com"
#endif

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
    settings.enabled = root["enabled"] | NTP_SETTINGS_SERVICE_FACTORY_ENABLED;
    settings.server = root["server"] | NTP_SETTINGS_SERVICE_FACTORY_SERVER;
    settings.tzLabel = root["tz_label"] | NTP_SETTINGS_SERVICE_FACTORY_TIME_ZONE_LABEL;
    settings.tzFormat = root["tz_format"] | NTP_SETTINGS_SERVICE_FACTORY_TIME_ZONE_FORMAT;
  }
};

class NTPSettingsService : public StatefulService<NTPSettings> {
 public:
  NTPSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();

 private:
  HttpEndpoint<NTPSettings> _httpEndpoint;
  FSPersistence<NTPSettings> _fsPersistence;

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
