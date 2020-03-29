#include <DemoProject.h>

static DemoSettingsSerializer SERIALIZER;
static DemoSettingsDeserializer DESERIALIZER;

static HomeAssistantSerializer HA_SERIALIZER;
static HomeAssistantDeserializer HA_DESERIALIZER;

DemoProject::DemoProject(AsyncWebServer* server,
                         FS* fs,
                         SecurityManager* securityManager,
                         AsyncMqttClient* mqttClient) :
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, DEMO_SETTINGS_PATH, securityManager),
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, DEMO_SETTINGS_FILE),
    _settingsBroker(&HA_SERIALIZER,
                    &HA_DESERIALIZER,
                    this,
                    mqttClient,
                    DEMO_SETTINGS_SET_TOPIC,
                    DEMO_SETTINGS_STATE_TOPIC),
    _mqttClient(mqttClient) {
  // configure blink led to be output
  pinMode(BLINK_LED, OUTPUT);

  // configure MQTT callback
  mqttClient->onConnect(std::bind(&DemoProject::registerConfig, this));

  // configure settings service update handler to update LED state
  addUpdateHandler([&](void* origin) { onConfigUpdated(); }, false);
}

void DemoProject::begin() {
  _settingsPersistence.readFromFS();
  onConfigUpdated();
}

void DemoProject::onConfigUpdated() {
  digitalWrite(BLINK_LED, _settings.ledOn ? LED_ON : LED_OFF);
}

void DemoProject::registerConfig() {
  Serial.println("Registering config....");

  DynamicJsonDocument doc(256);
  doc["~"] = "homeassistant/light/esp_demo";
  doc["name"] = "ESP Demo LED";
  doc["unique_id"] = "esp_demo_light";
  doc["cmd_t"] = "~/set";
  doc["stat_t"] = "~/state";
  doc["schema"] = "json";
  doc["brightness"] = false;
  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(DEMO_SETTINGS_CONFIG_TOPIC, 0, false, payload.c_str());
}