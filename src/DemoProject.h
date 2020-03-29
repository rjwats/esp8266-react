#ifndef DemoProject_h
#define DemoProject_h

#include <SettingsEndpoint.h>
#include <SettingsBroker.h>
#include <SettingsPersistence.h>
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

#define DEMO_SETTINGS_CONFIG_TOPIC "homeassistant/light/esp_demo/config"
#define DEMO_SETTINGS_SET_TOPIC "homeassistant/light/esp_demo/set"
#define DEMO_SETTINGS_STATE_TOPIC "homeassistant/light/esp_demo/state"

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

class HomeAssistantSerializer : public SettingsSerializer<DemoSettings> {
 public:
  void serialize(DemoSettings& settings, JsonObject root) {
    root["state"] = settings.ledOn ? ON_STATE : OFF_STATE;
  }
};

class HomeAssistantDeserializer : public SettingsDeserializer<DemoSettings> {
 public:
  void deserialize(DemoSettings& settings, JsonObject root) {
    String state = root["state"];
    settings.ledOn = strcmp(ON_STATE, state.c_str()) ? false : true;
  }
};

class DemoProject : public SettingsService<DemoSettings> {
 public:
  DemoProject(AsyncWebServer* server, FS* fs, SecurityManager* securityManager, AsyncMqttClient* mqttClient);
  void begin();

 private:
  SettingsEndpoint<DemoSettings> _settingsEndpoint;
  SettingsPersistence<DemoSettings> _settingsPersistence;
  SettingsBroker<DemoSettings> _settingsBroker;
  AsyncMqttClient* _mqttClient;
  void registerConfig();
  void onConfigUpdated();

};

#endif
