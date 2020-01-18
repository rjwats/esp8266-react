#include <DemoProject.h>

DemoProject::DemoProject(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    AdminSettingsService(server, fs, securityManager, DEMO_SETTINGS_PATH, DEMO_SETTINGS_FILE) {
  pinMode(BLINK_LED, OUTPUT);
}

DemoProject::~DemoProject() {
}

void printTm(const char* what, const tm* tm) {
  Serial.print(what);
  PTM(isdst);
  PTM(yday);
  PTM(wday);
  PTM(year);
  PTM(mon);
  PTM(mday);
  PTM(hour);
  PTM(min);
  PTM(sec);
}

void DemoProject::loop() {
  unsigned delay = MAX_DELAY / 255 * (255 - _blinkSpeed);
  unsigned long currentMillis = millis();
  if (!_lastBlink || (unsigned long)(currentMillis - _lastBlink) >= delay) {
    _lastBlink = currentMillis;
    digitalWrite(BLINK_LED, !digitalRead(BLINK_LED));
  }
  if (!_lastTimeEcho || (unsigned long)(currentMillis - _lastTimeEcho) >= 5000) {
    _lastTimeEcho = currentMillis;
    time_t now = time(nullptr);
    printTm("loc :", localtime(&now));
    Serial.println();
    printTm("utc :", gmtime(&now));
    Serial.println();
  }
}

void DemoProject::readFromJsonObject(JsonObject& root) {
  _blinkSpeed = root["blink_speed"] | DEFAULT_BLINK_SPEED;
}

void DemoProject::writeToJsonObject(JsonObject& root) {
  // connection settings
  root["blink_speed"] = _blinkSpeed;
}
