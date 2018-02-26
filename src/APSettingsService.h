#ifndef APSettingsConfig_h
#define APSettingsConfig_h

#include <IPAddress.h>
#include <SettingsService.h>

#define MANAGE_NETWORK_DELAY 10000

#define AP_MODE_ALWAYS 0
#define AP_MODE_DISCONNECTED 1
#define AP_MODE_NEVER 2

#define AP_DEFAULT_SSID "ssid"
#define AP_DEFAULT_PASSWORD "password"

#define AP_SETTINGS_FILE "/config/apSettings.json"
#define AP_SETTINGS_SERVICE_PATH "/apSettings"

class APSettingsService : public SettingsService {

  public:

    APSettingsService(AsyncWebServer* server, FS* fs);
    ~APSettingsService();

    void loop();

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);
    void onConfigUpdated();

  private:

    int _provisionMode;
    String _ssid;
    String _password;
    unsigned long _manageAtMillis;

};

#endif // end APSettingsConfig_h
