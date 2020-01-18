#ifndef NTPSettingsService_h
#define NTPSettingsService_h

#include <AdminSettingsService.h>

#include <sntp.h>
#include <time.h>
#include <coredecls.h>

// default time for experimenting
#define RTC_UTC_TEST 1510592825

// default time zone
#define NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_LABEL "Europe/London"
#define NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_FORMAT "GMT0BST,M3.5.0/1,M10.5.0"

// default time server
#define NTP_SETTINGS_SERVICE_DEFAULT_SERVER "time.google.com"

// min poll delay of 60 secs, max 1 day
#define NTP_SETTINGS_MIN_INTERVAL 60
#define NTP_SETTINGS_MAX_INTERVAL 86400

#define NTP_SETTINGS_FILE "/config/ntpSettings.json"
#define NTP_SETTINGS_SERVICE_PATH "/rest/ntpSettings"

class NTPSettingsService : public AdminSettingsService {
 public:
  NTPSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  ~NTPSettingsService();

  void loop();


 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
  void onConfigUpdated();
  void receivedNTPtime();

 private:
  String _tzLabel;
  String _tzFormat;
  String _server;

  bool _reconfigureNTP = false;

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
