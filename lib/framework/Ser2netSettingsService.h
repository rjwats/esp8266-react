#ifndef Ser2netSettingsService_h
#define Ser2netSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>

#ifndef FACTORY_SER2NET_ENABLED
#define FACTORY_SER2NET_ENABLED true
#endif

#ifndef FACTORY_SER2NET_TIME_ZONE_LABEL
#define FACTORY_SER2NET_TIME_ZONE_LABEL "Europe/London"
#endif

#ifndef FACTORY_SER2NET_TIME_ZONE_FORMAT
#define FACTORY_SER2NET_TIME_ZONE_FORMAT "GMT0BST,M3.5.0/1,M10.5.0"
#endif

#ifndef FACTORY_SER2NET_SERVER
#define FACTORY_SER2NET_SERVER "time.google.com"
#endif

#define SER2NET_SETTINGS_FILE "/config/ser2netSettings.json"
#define SER2NET_SETTINGS_SERVICE_PATH "/rest/ser2netSettings"

#define MAX_TIME_SIZE 256
#define TIME_PATH "/rest/time"

class Ser2netSettings {
 public:
  bool enabled;
  String tzLabel;
  String tzFormat;
  String server;

  static void read(Ser2netSettings& settings, JsonObject& root) {
    root["enabled"] = settings.enabled;
    root["server"] = settings.server;
    root["tz_label"] = settings.tzLabel;
    root["tz_format"] = settings.tzFormat;
  }

  static StateUpdateResult update(JsonObject& root, Ser2netSettings& settings) {
    settings.enabled = root["enabled"] | FACTORY_SER2NET_ENABLED;
    settings.server = root["server"] | FACTORY_SER2NET_SERVER;
    settings.tzLabel = root["tz_label"] | FACTORY_SER2NET_TIME_ZONE_LABEL;
    settings.tzFormat = root["tz_format"] | FACTORY_SER2NET_TIME_ZONE_FORMAT;
    return StateUpdateResult::CHANGED;
  }
};

class Ser2netSettingsService : public StatefulService<Ser2netSettings> {
 public:
  Ser2netSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();

 private:
  HttpEndpoint<Ser2netSettings> _httpEndpoint;
  FSPersistence<Ser2netSettings> _fsPersistence;
  AsyncCallbackJsonWebHandler _timeHandler;

#ifdef ESP32
  void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
  void onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ESP8266)
  WiFiEventHandler _onStationModeDisconnectedHandler;
  WiFiEventHandler _onStationModeGotIPHandler;

  void onStationModeGotIP(const WiFiEventStationModeGotIP& event);
  void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event);
#endif
  void configureSer2net();
  void configureTime(AsyncWebServerRequest* request, JsonVariant& json);
};

#endif  // end Ser2netSettingsService_h
