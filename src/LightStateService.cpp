#include <LightStateService.h>

LightStateService::LightStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     AsyncMqttClient* mqttClient,
                                     LightMqttSettingsService* lightMqttSettingsService) :
    _httpEndpoint(LightState::serialize,
                  LightState::deserialize,
                  this,
                  server,
                  LIGHT_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(LightState::haSerialize, LightState::haDeserialize, this, mqttClient),
    _webSocket(LightState::serialize,
               LightState::deserialize,
               this,
               server,
               LIGHT_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttClient(mqttClient),
    _lightMqttSettingsService(lightMqttSettingsService) {
  // configure blink led to be output
  pinMode(BLINK_LED, OUTPUT);

  // configure MQTT callback
  _mqttClient->onConnect(std::bind(&LightStateService::registerConfig, this));

  // configure update handler for when the light settings change
  _lightMqttSettingsService->addUpdateHandler([&](const String& originId) { registerConfig(); }, false);

  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void LightStateService::begin() {
  _state.ledOn = DEFAULT_LED_STATE;
  onConfigUpdated();
}

void LightStateService::onConfigUpdated() {
  digitalWrite(BLINK_LED, _state.ledOn ? LED_ON : LED_OFF);
}

void LightStateService::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String configTopic;
  String setTopic;
  String stateTopic;

  DynamicJsonDocument doc(256);
  _lightMqttSettingsService->read([&](LightMqttSettings& settings) {
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