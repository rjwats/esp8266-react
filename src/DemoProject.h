#ifndef DemoProject_h
#define DemoProject_h

#include <ArduinoJson.h>
#include <AdminSettingsService.h>
#include <ESP8266React.h>

#define BLINK_LED 2
#define PRINT_DELAY 5000

#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"
#define LED_ON 0x0
#define LED_OFF 0x1

#define DEMO_SETTINGS_FILE "/config/demoSettings.json"
#define DEMO_SETTINGS_PATH "/rest/demoSettings"

class DemoSettings {
 public:
  bool ledOn;
};

class DemoProject : public AdminSettingsService<DemoSettings> {
 public:
  DemoProject(AsyncWebServer* server, FS* fs, SecurityManager* securityManager, AsyncMqttClient* mqttClient);
  ~DemoProject();

  void loop();
  void begin();

 private:
  unsigned long _lastPrint = 0;
  AsyncMqttClient* _mqttClient;
  void configureMQTT();
  void publishState();
  void onMqttMessage(char* topic,
                     char* payload,
                     AsyncMqttClientMessageProperties properties,
                     size_t len,
                     size_t index,
                     size_t total);

 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
  void onConfigUpdated();
};

#endif
