#include <DemoProject.h>

DemoProject::DemoProject(AsyncWebServer* server, FS* fs) :
    AdminSettingsService(server, fs, esp8266React->getSecurityManager(), DEMO_SETTINGS_PATH, DEMO_SETTINGS_FILE) {
  pinMode(BLINK_LED, OUTPUT);
}

DemoProject::~DemoProject() {
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
