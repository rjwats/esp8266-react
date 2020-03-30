#ifndef DemoProject_h
#define DemoProject_h

#include <LightSettingsService.h>
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
  DemoProject(AsyncWebServer* server,
              SecurityManager* securityManager,
              AsyncMqttClient* mqttClient,
              LightSettingsService* lightSettingsService);
  void begin();

 private:
  SettingsEndpoint<DemoSettings> _settingsEndpoint;
  SettingsBroker<DemoSettings> _settingsBroker;
  AsyncMqttClient* _mqttClient;
  LightSettingsService* _lightSettingsService;

  void registerConfig();
  void onConfigUpdated();
};

#endif
