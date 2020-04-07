#ifndef APSettingsConfig_h
#define APSettingsConfig_h

#include <SettingsEndpoint.h>
#include <SettingsPersistence.h>

#include <DNSServer.h>
#include <IPAddress.h>

#define MANAGE_NETWORK_DELAY 10000

#define AP_MODE_ALWAYS 0
#define AP_MODE_DISCONNECTED 1
#define AP_MODE_NEVER 2

#define DNS_PORT 53

#define AP_DEFAULT_SSID "ESP8266-React"
#define AP_DEFAULT_PASSWORD "esp-react"

#define AP_SETTINGS_FILE "/config/apSettings.json"
#define AP_SETTINGS_SERVICE_PATH "/rest/apSettings"

class APSettings {
 public:
  uint8_t provisionMode;
  String ssid;
  String password;
};

class APSettingsSerializer : public SettingsSerializer<APSettings> {
 public:
  void serialize(APSettings& settings, JsonObject root) {
    root["provision_mode"] = settings.provisionMode;
    root["ssid"] = settings.ssid;
    root["password"] = settings.password;
  }
};

class APSettingsDeserializer : public SettingsDeserializer<APSettings> {
 public:
  void deserialize(APSettings& settings, JsonObject root) {
    settings.provisionMode = root["provision_mode"] | AP_MODE_ALWAYS;
    switch (settings.provisionMode) {
      case AP_MODE_ALWAYS:
      case AP_MODE_DISCONNECTED:
      case AP_MODE_NEVER:
        break;
      default:
        settings.provisionMode = AP_MODE_ALWAYS;
    }
    settings.ssid = root["ssid"] | AP_DEFAULT_SSID;
    settings.password = root["password"] | AP_DEFAULT_PASSWORD;
  }
};

class APSettingsService : public SettingsService<APSettings> {
 public:
  APSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();
  void loop();

 private:
  SettingsEndpoint<APSettings> _settingsEndpoint;
  SettingsPersistence<APSettings> _settingsPersistence;

  // for the mangement delay loop
  unsigned long _lastManaged;

  // for the captive portal
  DNSServer* _dnsServer;

  void reconfigureAP();
  void manageAP();
  void startAP();
  void stopAP();
  void handleDNS();
};

#endif  // end APSettingsConfig_h