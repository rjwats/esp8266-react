#include <DemoProject.h>

DemoProject::DemoProject(AsyncWebServer* server,
                         FS* fs,
                         SecurityManager* securityManager,
                         AsyncMqttClient* mqttClient) :
    AdminSettingsService(server, fs, securityManager, DEMO_SETTINGS_PATH, DEMO_SETTINGS_FILE),
    _mqttClient(mqttClient) {
  pinMode(BLINK_LED, OUTPUT);
  mqttClient->onConnect(std::bind(&DemoProject::registerConfig, this, std::placeholders::_1));
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

void DemoProject::registerConfig(bool sessionPresent) {
  _mqttClient->publish(
      "homeassistant/light/kitchen/config",
      0,
      true,
      "{\"~\": \"homeassistant/light/kitchen\",\"name\": \"Demo Project\",\"unique_id\": \"kitchen_light\",\"cmd_t\": "
      "\"~/set\",\"stat_t\": \"~/state\",\"schema\": \"json\",\"brightness\": false}");
  _mqttClient->publish("homeassistant/light/kitchen/state", 0, false, "{\"state\": \"ON\"}");
  _mqttClient->subscribe("homeassistant/light/kitchen/set", 2);
}

void DemoProject::onMqttMessage(char* topic,
                   char* payload,
                   AsyncMqttClientMessageProperties properties,
                   size_t len,
                   size_t index,
                   size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
}

void DemoProject::loop() {
  unsigned delay = MAX_DELAY / 255 * (255 - _settings.blinkSpeed);
  unsigned long currentMillis = millis();
  if (!_lastBlink || (unsigned long)(currentMillis - _lastBlink) >= delay) {
    _lastBlink = currentMillis;
    digitalWrite(BLINK_LED, !digitalRead(BLINK_LED));
  }
}

void DemoProject::readFromJsonObject(JsonObject& root) {
  _settings.blinkSpeed = root["blink_speed"] | DEFAULT_BLINK_SPEED;
}

void DemoProject::writeToJsonObject(JsonObject& root) {
  // connection settings
  root["blink_speed"] = _settings.blinkSpeed;
}
