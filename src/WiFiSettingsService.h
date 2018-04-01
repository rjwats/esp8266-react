#ifndef WiFiSettingsService_h
#define WiFiSettingsService_h

#include <IPAddress.h>
#include <SettingsService.h>

#define WIFI_SETTINGS_FILE "/config/wifiSettings.json"
#define WIFI_SETTINGS_SERVICE_PATH "/rest/wifiSettings"

class WiFiSettingsService : public SettingsService {

  public:

    WiFiSettingsService(AsyncWebServer* server, FS* fs);
    ~WiFiSettingsService();

    void begin();

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);
    void onConfigUpdated();

    void reconfigureWiFiConnection();

  private:
    // connection settings
    String _ssid;
    String _password;
    String _hostname;
    bool _staticIPConfig;

    // optional configuration for static IP address
    IPAddress _localIP;
    IPAddress _gatewayIP;
    IPAddress _subnetMask;
    IPAddress _dnsIP1;
    IPAddress _dnsIP2;

    void readIP(JsonObject& root, String key, IPAddress& _ip);
    void writeIP(JsonObject& root, String key, IPAddress& _ip);

};

#endif // end WiFiSettingsService_h
