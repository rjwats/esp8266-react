#ifndef LightSettingsService_h
#define LightSettingsService_h

#include <SettingsEndpoint.h>
#include <SettingsBroker.h>
#include <SettingsPersistence.h>
#include <ESP8266React.h>

#define LIGHT_SETTINGS_FILE "/config/lightSettings.json"
#define LIGHT_SETTINGS_PATH "/rest/lightSettings"

class LightSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;
};

static String defaultDeviceValue(String prefix = "") {
#ifdef ESP32
  return prefix + String((unsigned long)ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
  return prefix + String(ESP.getChipId(), HEX);
#endif
}

class LightSettingsSerializer : public SettingsSerializer<LightSettings> {
 public:
  void serialize(LightSettings& settings, JsonObject root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }
};

class LightSettingsDeserializer : public SettingsDeserializer<LightSettings> {
 public:
  void deserialize(LightSettings& settings, JsonObject root) {
    settings.mqttPath = root["mqtt_path"] | defaultDeviceValue("homeassistant/light/");
    settings.name = root["name"] | defaultDeviceValue("light-");
    settings.uniqueId = root["unique_id"] | defaultDeviceValue("light-");
  }
};

class LightSettingsService : public SettingsService<LightSettings> {
 public:
  LightSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  SettingsEndpoint<LightSettings> _settingsEndpoint;
  SettingsPersistence<LightSettings> _settingsPersistence;
};

#endif  // end LightSettingsService_h
