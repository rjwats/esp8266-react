#ifndef APSettingsConfig_h
#define APSettingsConfig_h

#include <AdminSettingsService.h>
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

class APSettingsService : public AdminSettingsService {
 public:
  APSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  ~APSettingsService();

  void begin();
  void loop();

 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
  void onConfigUpdated();

 private:
  // access point settings
  uint8_t _provisionMode;
  String _ssid;
  String _password;

  // for the mangement delay loop
  unsigned long _lastManaged;

  // for the captive portal
  DNSServer* _dnsServer;

  void manageAP();
  void startAP();
  void stopAP();
  void handleDNS();
};

#endif  // end APSettingsConfig_h