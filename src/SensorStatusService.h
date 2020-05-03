#ifndef SensorStatusService_h
#define SensorStatusService_h

#include <SensorSettingsService.h>
#include <SettingsGetEndpoint.h>
#include <SettingsStateBroker.h>
#include <SettingsSocket.h>
#include <ESP8266React.h>

#include <Wire.h>
#include <SPI.h>
#include <DHT.h>

#define DHT_PIN 4
#define DHT_TYPE DHT22

#define PRINT_DELAY 5000

#define SENSOR_STATUS_ENDPOINT_PATH "/rest/sensorStatus"
// #define SENSOR_STATUS_SOCKET_PATH "/ws/sensorStatus"

class SensorStatus {
 public:
  float temperature;
  float humidity;
};

class SensorStatusSerializer : public SettingsSerializer<SensorStatus> {
 public:
  void serialize(SensorStatus& settings, JsonObject root) {
    root["temperature"] = settings.temperature;
    root["humidity"] = settings.humidity;
  }
};

class SensorStatusService : public SettingsService<SensorStatus> {
 public:
  SensorStatusService(AsyncWebServer* server,
                      SecurityManager* securityManager,
                      AsyncMqttClient* mqttClient,
                      SensorSettingsService* sensorSettingsService);
  void begin();
  void loop();

 private:
  SettingsGetEndpoint<SensorStatus> _settingsEndpoint;
  SettingsStateBroker<SensorStatus> _settingsBroker;
  AsyncMqttClient* _mqttClient;
  SensorSettingsService* _sensorSettingsService;
  DHT _dht;
  unsigned long _lastReading = 0;

  // SettingsSocket<SensorStatus> _settingsSocket;
  /*_settingsSocket(&SERIALIZER,
                &DESERIALIZER,
                this,
                server,
                LIGHT_SETTINGS_SOCKET_PATH,
                securityManager,
                AuthenticationPredicates::IS_AUTHENTICATED),*/

  void registerConfig();
  void onConfigUpdated();
};

#endif
