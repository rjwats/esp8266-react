#ifndef DemoProject_h
#define DemoProject_h

#include <SettingsEndpoint.h>
#include <SettingsPersistence.h>
#include <ESP8266React.h>

#define BLINK_LED 2
#define MAX_DELAY 1000

#define DEFAULT_BLINK_SPEED 100
#define DEMO_SETTINGS_FILE "/config/demoSettings.json"
#define DEMO_SETTINGS_PATH "/rest/demoSettings"

class DemoSettings {
 public:
  uint8_t blinkSpeed;
};

class DemoSettingsSerializer : public SettingsSerializer<DemoSettings> {
 public:
  void serialize(DemoSettings& settings, JsonObject root) {
    root["blink_speed"] = settings.blinkSpeed;
  }
};

class DemoSettingsDeserializer : public SettingsDeserializer<DemoSettings> {
 public:
  void deserialize(DemoSettings& settings, JsonObject root) {
    settings.blinkSpeed = root["blink_speed"] | DEFAULT_BLINK_SPEED;
  }
};

class DemoProject : public SettingsService<DemoSettings> {
 public:
  DemoProject(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();
  void loop();

 private:
  SettingsEndpoint<DemoSettings> _settingsEndpoint;
  SettingsPersistence<DemoSettings> _settingsPersistence;

  unsigned long _lastBlink = 0;
};

#endif
