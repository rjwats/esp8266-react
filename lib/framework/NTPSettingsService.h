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

#ifndef FACTORY_NTP_ENABLED
#define FACTORY_NTP_ENABLED true
#endif

#ifndef FACTORY_NTP_TIME_ZONE_LABEL
#define FACTORY_NTP_TIME_ZONE_LABEL "Europe/London"
#endif

#ifndef FACTORY_NTP_TIME_ZONE_FORMAT
#define FACTORY_NTP_TIME_ZONE_FORMAT "GMT0BST,M3.5.0/1,M10.5.0"
#endif

#ifndef FACTORY_NTP_SERVER
#define FACTORY_NTP_SERVER "time.google.com"
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
    settings.enabled = root["enabled"] | FACTORY_NTP_ENABLED;
    settings.server = root["server"] | FACTORY_NTP_SERVER;
    settings.tzLabel = root["tz_label"] | FACTORY_NTP_TIME_ZONE_LABEL;
    settings.tzFormat = root["tz_format"] | FACTORY_NTP_TIME_ZONE_FORMAT;
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
