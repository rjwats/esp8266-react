#ifndef SensorSettingsService_h
#define SensorSettingsService_h

#include <SettingsEndpoint.h>
#include <SettingsBroker.h>
#include <SettingsPersistence.h>
#include <ESP8266React.h>
#include <util.h>

#define SENSOR_SETTINGS_FILE "/config/sensorSettings.json"
#define SENSOR_SETTINGS_PATH "/rest/sensorSettings"

class SensorSettings {
 public:
  String mqttPath;
  String uniqueId;
};

class SensorSettingsSerializer : public SettingsSerializer<SensorSettings> {
 public:
  void serialize(SensorSettings& settings, JsonObject root) {
    root["mqtt_path"] = settings.mqttPath;
    root["unique_id"] = settings.uniqueId;
  }
};

class SensorSettingsDeserializer : public SettingsDeserializer<SensorSettings> {
 public:
  void deserialize(SensorSettings& settings, JsonObject root) {
    settings.mqttPath = root["mqtt_path"] | defaultDeviceValue("homeassistant/sensor/garden");
    settings.uniqueId = root["unique_id"] | defaultDeviceValue("sensor-");
  }
};

class SensorSettingsService : public SettingsService<SensorSettings> {
 public:
  SensorSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  SettingsEndpoint<SensorSettings> _settingsEndpoint;
  SettingsPersistence<SensorSettings> _settingsPersistence;
};

#endif  // end SensorSettingsService_h
