#include <DemoProject.h>

void DemoProject::init(AsyncWebServer* server) {
  AdminSettingsService::init(server);
  pinMode(BLINK_LED, OUTPUT);  
}

void DemoProject::loop() {
  unsigned delay = MAX_DELAY / 255 * (255 - _blinkSpeed);
  unsigned long currentMillis = millis();
  if (!_lastBlink || (unsigned long)(currentMillis - _lastBlink) >= delay) {
    _lastBlink = currentMillis;
    digitalWrite(BLINK_LED, !digitalRead(BLINK_LED));
  }
}

void DemoProject::readFromJsonObject(JsonObject& root) {
  _blinkSpeed = root["blink_speed"] | DEFAULT_BLINK_SPEED;
}

void DemoProject::writeToJsonObject(JsonObject& root) {
  // connection settings
  root["blink_speed"] = _blinkSpeed;
}

