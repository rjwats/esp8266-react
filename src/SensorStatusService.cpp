#include <SensorStatusService.h>

static SensorStatusSerializer SERIALIZER;

SensorStatusService::SensorStatusService(AsyncWebServer* server,
                                         SecurityManager* securityManager,
                                         AsyncMqttClient* mqttClient,
                                         SensorSettingsService* sensorSettingsService) :
    _settingsEndpoint(&SERIALIZER,
                      this,
                      server,
                      SENSOR_STATUS_ENDPOINT_PATH,
                      securityManager,
                      AuthenticationPredicates::IS_AUTHENTICATED),
    _settingsBroker(&SERIALIZER, this, mqttClient),

    _mqttClient(mqttClient),
    _sensorSettingsService(sensorSettingsService),
    _dht(DHT_PIN, DHT_TYPE) {
  _mqttClient->onConnect(std::bind(&SensorStatusService::registerConfig, this));
  _sensorSettingsService->addUpdateHandler([&](String originId) { registerConfig(); }, false);
}

void SensorStatusService::begin() {
  _dht.begin();
}

void SensorStatusService::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastReading);
  if (manageElapsed >= 5000) {
    _lastReading = currentMillis;

    float humidity = _dht.readHumidity();
    float temperature = _dht.readTemperature();
    if (!isnan(humidity) && !isnan(temperature)) {
      update(
          [&](SensorStatus& settings) {
            settings.humidity = humidity;
            settings.temperature = temperature;
          },
          "sensor");
    }
  }
}

void SensorStatusService::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String stateTopic;
  String temperatureTopic;
  String humidityTopic;

  DynamicJsonDocument temperatureSensorDoc(256);
  DynamicJsonDocument humiditySensorDoc(256);

  _sensorSettingsService->read([&](SensorSettings& settings) {
    stateTopic = settings.mqttPath + "/state";

    temperatureTopic = settings.mqttPath + "/temperature/config";
    temperatureSensorDoc["unique_id"] = settings.uniqueId + "_temperature";
    temperatureSensorDoc["name"] = "Temperature";
    temperatureSensorDoc["device_class"] = "temperature";
    temperatureSensorDoc["state_topic"] = stateTopic;
    temperatureSensorDoc["unit_of_measurement"] = "°C";
    temperatureSensorDoc["value_template"] = "{{ value_json.temperature}}";

    humidityTopic = settings.mqttPath + "/humidity/config";
    humiditySensorDoc["unique_id"] = settings.uniqueId + "_humidity";
    humiditySensorDoc["name"] = "Humidity";
    humiditySensorDoc["device_class"] = "humidity";
    humiditySensorDoc["state_topic"] = stateTopic;
    humiditySensorDoc["unit_of_measurement"] = "%";
    humiditySensorDoc["value_template"] = "{{ value_json.humidity}}";
  });

  String payload;
  serializeJson(temperatureSensorDoc, payload);
  _mqttClient->publish(temperatureTopic.c_str(), 0, false, payload.c_str());

  payload.clear();
  serializeJson(humiditySensorDoc, payload);
  _mqttClient->publish(humidityTopic.c_str(), 0, false, payload.c_str());

  _settingsBroker.setTopic(stateTopic);
}