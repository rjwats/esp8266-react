#ifndef APSettingsConfig_h
#define APSettingsConfig_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>

#include <DNSServer.h>
#include <IPAddress.h>

#define MANAGE_NETWORK_DELAY 10000

#define AP_MODE_ALWAYS 0
#define AP_MODE_DISCONNECTED 1
#define AP_MODE_NEVER 2

#define DNS_PORT 53

#ifndef FACTORY_AP_SSID
  #define FACTORY_AP_SSID "ESP8266-React"
#endif

#ifndef FACTORY_AP_PASSWORD
  #define FACTORY_AP_PASSWORD "esp-react"
#endif

#define AP_SETTINGS_FILE "/config/apSettings.json"
#define AP_SETTINGS_SERVICE_PATH "/rest/apSettings"

class APSettings {
 public:
  uint8_t provisionMode;
  String ssid;
  String password;

  static void serialize(APSettings& settings, JsonObject& root) {
    root["provision_mode"] = settings.provisionMode;
    root["ssid"] = settings.ssid;
    root["password"] = settings.password;
  }

  static void deserialize(JsonObject& root, APSettings& settings) {
    settings.provisionMode = root["provision_mode"] | AP_MODE_ALWAYS;
    switch (settings.provisionMode) {
      case AP_MODE_ALWAYS:
      case AP_MODE_DISCONNECTED:
      case AP_MODE_NEVER:
        break;
      default:
        settings.provisionMode = AP_MODE_ALWAYS;
    }
    settings.ssid = root["ssid"] | FACTORY_AP_SSID;
    settings.password = root["password"] | FACTORY_AP_PASSWORD;
  }
};

class APSettingsService : public StatefulService<APSettings> {
 public:
  APSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();
  void loop();

 private:
  HttpEndpoint<APSettings> _httpEndpoint;
  FSPersistence<APSettings> _fsPersistence;

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