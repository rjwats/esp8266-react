#ifndef LightMqttSettingsService_h
#define LightMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPUtils.h>

#define LIGHT_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define LIGHT_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class LightMqttSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void read(LightMqttSettings& settings, JsonObject& root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }

  static StateUpdateResult update(JsonObject& root, LightMqttSettings& settings) {
    settings.mqttPath = root["mqtt_path"] | ESPUtils::defaultDeviceValue("homeassistant/light/");
    settings.name = root["name"] | ESPUtils::defaultDeviceValue("light-");
    settings.uniqueId = root["unique_id"] | ESPUtils::defaultDeviceValue("light-");
    return StateUpdateResult::CHANGED;
  }
};

class LightMqttSettingsService : public StatefulService<LightMqttSettings> {
 public:
  LightMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<LightMqttSettings> _httpEndpoint;
  FSPersistence<LightMqttSettings> _fsPersistence;
};

#endif  // end LightMqttSettingsService_h
