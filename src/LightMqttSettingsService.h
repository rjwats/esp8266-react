#ifndef LightMqttSettingsService_h
#define LightMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>

#define LIGHT_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define LIGHT_BROKER_SETTINGS_PATH "/rest/brokerSettings"

static String defaultDeviceValue(String prefix = "") {
#ifdef ESP32
  return prefix + String((unsigned long)ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
  return prefix + String(ESP.getChipId(), HEX);
#endif
}

class LightMqttSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void serialize(LightMqttSettings& settings, JsonObject& root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }

  static void deserialize(JsonObject& root, LightMqttSettings& settings) {
    settings.mqttPath = root["mqtt_path"] | defaultDeviceValue("homeassistant/light/");
    settings.name = root["name"] | defaultDeviceValue("light-");
    settings.uniqueId = root["unique_id"] | defaultDeviceValue("light-");
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
