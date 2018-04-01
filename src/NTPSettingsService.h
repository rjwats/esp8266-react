#ifndef NTPSettingsService_h
#define NTPSettingsService_h

#include <SettingsService.h>
#include <ESP8266WiFi.h>

#include <TimeLib.h>
#include <NtpClientLib.h>

// default time server
#define NTP_SETTINGS_SERVICE_DEFAULT_SERVER "pool.ntp.org"
#define NTP_SETTINGS_SERVICE_DEFAULT_INTERVAL 3600

// min poll delay of 60 secs, max 1 day
#define NTP_SETTINGS_MIN_INTERVAL 60
#define NTP_SETTINGS_MAX_INTERVAL 86400

#define NTP_SETTINGS_FILE "/config/ntpSettings.json"
#define NTP_SETTINGS_SERVICE_PATH "/rest/ntpSettings"

class NTPSettingsService : public SettingsService {

  public:

    NTPSettingsService(AsyncWebServer* server, FS* fs);
    ~NTPSettingsService();

    void loop();

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);
    void onConfigUpdated();

  private:

    WiFiEventHandler _onStationModeDisconnectedHandler;
    WiFiEventHandler _onStationModeGotIPHandler;

    String _server;
    int _interval;

    bool _reconfigureNTP = false;
    bool _syncEventTriggered = false;
    NTPSyncEvent_t _ntpEvent;

    void onStationModeGotIP(const WiFiEventStationModeGotIP& event);
    void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event);

    void configureNTP();
    void processSyncEvent(NTPSyncEvent_t ntpEvent);

};

#endif // end NTPSettingsService_h
