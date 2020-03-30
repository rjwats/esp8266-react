#include <DemoProject.h>

static DemoSettingsSerializer SERIALIZER;
static DemoSettingsDeserializer DESERIALIZER;

static HomeAssistantSerializer HA_SERIALIZER;
static HomeAssistantDeserializer HA_DESERIALIZER;

DemoProject::DemoProject(AsyncWebServer* server,
                         SecurityManager* securityManager,
                         AsyncMqttClient* mqttClient,
                         LightSettingsService* lightSettingsService) :
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, DEMO_SETTINGS_ENDPOINT_PATH, securityManager),
    _settingsBroker(&HA_SERIALIZER, &HA_DESERIALIZER, this, mqttClient),
    _settingsSocket(&SERIALIZER, &DESERIALIZER, this, server, DEMO_SETTINGS_SOCKET_PATH),
    _mqttClient(mqttClient),
    _lightSettingsService(lightSettingsService) {
  // configure blink led to be output
  pinMode(BLINK_LED, OUTPUT);

  // configure MQTT callback
  _mqttClient->onConnect(std::bind(&DemoProject::registerConfig, this));

  // configure update handler for when the light settings change
  _lightSettingsService->addUpdateHandler([&](void* origin) { registerConfig(); }, false);

  // configure settings service update handler to update LED state
  addUpdateHandler([&](void* origin) { onConfigUpdated(); }, false);
}

void DemoProject::begin() {
  _settings.ledOn = DEFAULT_LED_STATE;
  onConfigUpdated();
}

void DemoProject::onConfigUpdated() {
  digitalWrite(BLINK_LED, _settings.ledOn ? LED_ON : LED_OFF);
}

void DemoProject::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String configTopic;
  String setTopic;
  String stateTopic;

  DynamicJsonDocument doc(256);
  _lightSettingsService->read([&](LightSettings& settings) {
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

  _settingsBroker.configureBroker(setTopic, stateTopic);
}