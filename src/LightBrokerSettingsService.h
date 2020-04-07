#ifndef LightBrokerSettingsService_h
#define LightBrokerSettingsService_h

#include <SettingsEndpoint.h>
#include <SettingsBroker.h>
#include <SettingsPersistence.h>
#include <ESP8266React.h>

#define LIGHT_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define LIGHT_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class LightBrokerSettings {
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

class LightBrokerSettingsSerializer : public SettingsSerializer<LightBrokerSettings> {
 public:
  void serialize(LightBrokerSettings& settings, JsonObject root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }
};

class LightBrokerSettingsDeserializer : public SettingsDeserializer<LightBrokerSettings> {
 public:
  void deserialize(LightBrokerSettings& settings, JsonObject root) {
    settings.mqttPath = root["mqtt_path"] | defaultDeviceValue("homeassistant/light/");
    settings.name = root["name"] | defaultDeviceValue("light-");
    settings.uniqueId = root["unique_id"] | defaultDeviceValue("light-");
  }
};

class LightBrokerSettingsService : public SettingsService<LightBrokerSettings> {
 public:
  LightBrokerSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  SettingsEndpoint<LightBrokerSettings> _settingsEndpoint;
  SettingsPersistence<LightBrokerSettings> _settingsPersistence;
};

#endif  // end LightBrokerSettingsService_h
