#include <DemoProject.h>

DemoProject::DemoProject(AsyncWebServer* server,
                         FS* fs,
                         SecurityManager* securityManager,
                         AsyncMqttClient* mqttClient) :
    AdminSettingsService(server, fs, securityManager, DEMO_SETTINGS_PATH, DEMO_SETTINGS_FILE),
    _mqttClient(mqttClient) {
  pinMode(BLINK_LED, OUTPUT);
  mqttClient->onConnect(std::bind(&DemoProject::configureMQTT, this));
  mqttClient->onMessage(std::bind(&DemoProject::onMqttMessage,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3,
                                  std::placeholders::_4,
                                  std::placeholders::_5,
                                  std::placeholders::_6));
}

DemoProject::~DemoProject() {
}

/**
 * Load persisted settings and led accordingly
 */
void DemoProject::begin() {
  AdminSettingsService::begin();
  onConfigUpdated();
}

/*
 * Periodically logs the LED state.
 */
void DemoProject::loop() {
  unsigned long currentMillis = millis();
  if (!_lastPrint || (unsigned long)(currentMillis - _lastPrint) >= PRINT_DELAY) {
    Serial.print("LED state is: ");
    Serial.println(_settings.ledOn ? ON_STATE : OFF_STATE);
    _lastPrint = currentMillis;
  }
}

void DemoProject::readFromJsonObject(JsonObject& root) {
  _settings.ledOn = root["led_on"] | DEFAULT_LED_STATE;
}

void DemoProject::writeToJsonObject(JsonObject& root) {
  root["led_on"] = _settings.ledOn;
}

void DemoProject::onConfigUpdated() {
  digitalWrite(BLINK_LED, _settings.ledOn ? LED_ON : LED_OFF);
  publishState();
}

/**
 * Publishes a topic to the MQTT broker, designed for homeassistants auto-discovery feature.
 */
void DemoProject::configureMQTT() {
  Serial.println("Registering config.");

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
  _mqttClient->publish("homeassistant/light/esp_demo/config", 0, false, payload.c_str());
  _mqttClient->subscribe("homeassistant/light/esp_demo/set", 2);

  publishState();
}

/*
 * Publish the state update to home assistant when the state of the LED changes
 */
void DemoProject::publishState() {
  if (_mqttClient->connected()) {
    DynamicJsonDocument doc(256);
    doc["state"] = _settings.ledOn ? ON_STATE : OFF_STATE;

    String payload;
    serializeJson(doc, payload);

    _mqttClient->publish("homeassistant/light/esp_demo/state", 0, false, payload.c_str());
  }
}

void DemoProject::onMqttMessage(char* topic,
                                char* payload,
                                AsyncMqttClientMessageProperties properties,
                                size_t len,
                                size_t index,
                                size_t total) {
  // we only care about the topic we are watching in this class
  if (strcmp("homeassistant/light/esp_demo/set", topic)) {
    return;
  }

  // convert homeassistant's "ON" and "OFF" states to a boolean
  DynamicJsonDocument doc(256);
  DeserializationError err = deserializeJson(doc, payload, len);
  if (!err) {
    String state = doc["state"];
    _settings.ledOn = strcmp(ON_STATE, state.c_str()) ? false : true;
    onConfigUpdated();
  }
}
