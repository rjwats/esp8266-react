#ifndef DemoProject_h
#define DemoProject_h

#include <SettingsEndpoint.h>
#include <SettingsPersistence.h>
#include <SettingsSerializer.h>
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

class DemoSettingsSerializer : public SettingsSerializer<DemoSettings> {
 public:
  void serialize(DemoSettings& settings, JsonObject root) {
    root["led_on"] = settings.ledOn;
  }
};

class DemoSettingsDeserializer : public SettingsDeserializer<DemoSettings> {
 public:
  void deserialize(DemoSettings& settings, JsonObject root) {
    settings.ledOn = root["led_on"] | DEFAULT_LED_STATE;
  }
};

class DemoProject : public SettingsService<DemoSettings> {
 public:
  DemoProject(AsyncWebServer* server, FS* fs, SecurityManager* securityManager, AsyncMqttClient* mqttClient);

  void begin();

 private:
  SettingsEndpoint<DemoSettings> _settingsEndpoint;
  SettingsPersistence<DemoSettings> _settingsPersistence;
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
  void onConfigUpdated();
};

#endif
