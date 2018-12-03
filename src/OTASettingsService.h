#ifndef OTASettingsService_h
#define OTASettingsService_h

#include <SettingsService.h>

#if defined(ESP8266)
  #include <ESP8266mDNS.h>
#elif defined(ESP_PLATFORM)
  #include <ESPmDNS.h>
#endif

#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Emergency defaults
#define DEFAULT_OTA_PORT 8266
#define DEFAULT_OTA_PASSWORD "esp-react"

#define OTA_SETTINGS_FILE "/config/otaSettings.json"
#define OTA_SETTINGS_SERVICE_PATH "/rest/otaSettings"

class OTASettingsService : public SettingsService {

  public:

    OTASettingsService(AsyncWebServer* server, FS* fs);
    ~OTASettingsService();

    void loop();

  protected:

      void onConfigUpdated();
      void readFromJsonObject(JsonObject& root);
      void writeToJsonObject(JsonObject& root);

  private:

    ArduinoOTAClass *_arduinoOTA;
    bool _enabled;
    int _port;
    String _password;

    void configureArduinoOTA();

#if defined(ESP8266)
    WiFiEventHandler _onStationModeGotIPHandler;    
    void onStationModeGotIP(const WiFiEventStationModeGotIP& event);
#elif defined(ESP_PLATFORM)
    void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);    
#endif    

};

#endif // end NTPSettingsService_h
