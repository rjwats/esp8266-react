#ifndef OTASettingsService_h
#define OTASettingsService_h

#include <SettingsService.h>
#include <ESP8266WiFi.h> // ??
#include <ESP8266mDNS.h>
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

    void begin();
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

};

#endif // end NTPSettingsService_h
