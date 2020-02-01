#ifndef WiFiSettingsService_h
#define WiFiSettingsService_h

#include <AdminSettingsService.h>
#include <IPAddress.h>

#define WIFI_SETTINGS_FILE "/config/wifiSettings.json"
#define WIFI_SETTINGS_SERVICE_PATH "/rest/wifiSettings"
#define WIFI_RECONNECTION_DELAY 1000 * 30

class WiFiSettings {
 public:
  // core wifi configuration
  String ssid;
  String password;
  String hostname;
  bool staticIPConfig;

  // optional configuration for static IP address
  IPAddress localIP;
  IPAddress gatewayIP;
  IPAddress subnetMask;
  IPAddress dnsIP1;
  IPAddress dnsIP2;
};

class WiFiSettingsService : public AdminSettingsService<WiFiSettings> {
 public:
  WiFiSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  ~WiFiSettingsService();

  void begin();
  void loop();

 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
  void onConfigUpdated();

 private:
  unsigned long _lastConnectionAttempt;

#ifdef ESP32
  bool _stopping;
  void onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
  void onStationModeStop(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ESP8266)
  WiFiEventHandler _onStationModeDisconnectedHandler;
  void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event);
#endif

  void readIP(JsonObject& root, String key, IPAddress& _ip);
  void writeIP(JsonObject& root, String key, IPAddress& _ip);
  void reconfigureWiFiConnection();
  void manageSTA();
};

#endif  // end WiFiSettingsService_h
