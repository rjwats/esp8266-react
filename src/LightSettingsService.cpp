#include <LightSettingsService.h>

LightSettingsService::LightSettingsService(AsyncWebServer* server,
                                           SecurityManager* securityManager,
                                           AsyncMqttClient* mqttClient,
                                           LightBrokerSettingsService* lightBrokerSettingsService) :
    _httpEndpoint(LightSettings::serialize,
                  LightSettings::deserialize,
                  this,
                  server,
                  LIGHT_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(LightSettings::haSerialize, LightSettings::haDeserialize, this, mqttClient),
    _webSocket(LightSettings::serialize,
               LightSettings::deserialize,
               this,
               server,
               LIGHT_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttClient(mqttClient),
    _lightBrokerSettingsService(lightBrokerSettingsService) {
  // configure blink led to be output
  pinMode(BLINK_LED, OUTPUT);

  // configure MQTT callback
  _mqttClient->onConnect(std::bind(&LightSettingsService::registerConfig, this));

  // configure update handler for when the light settings change
  _lightBrokerSettingsService->addUpdateHandler([&](String originId) { registerConfig(); }, false);

  // configure settings service update handler to update LED state
  addUpdateHandler([&](String originId) { onConfigUpdated(); }, false);
}

void LightSettingsService::begin() {
  _settings.ledOn = DEFAULT_LED_STATE;
  onConfigUpdated();
}

void LightSettingsService::onConfigUpdated() {
  digitalWrite(BLINK_LED, _settings.ledOn ? LED_ON : LED_OFF);
}

void LightSettingsService::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String configTopic;
  String setTopic;
  String stateTopic;

  DynamicJsonDocument doc(256);
  _lightBrokerSettingsService->read([&](LightBrokerSettings& settings) {
    configTopic = settings.mqttPath + "/config";
    setTopic = settings.mqttPath + "/set";
    stateTopic = settings.mqttPath + "/state";
    doc["~"] = settings.mqttPath;
    doc["name"] = settings.name;
    doc["unique_id"] = settings.uniqueId;
  });
  doc["cmd_t"] = "~/set";
  doc["stat_t"] = "~/state";
  doc["schema"] = "json";
  doc["brightness"] = false;

  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

  _mqttPubSub.configureTopics(stateTopic, setTopic);
}