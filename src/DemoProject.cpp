#include <DemoProject.h>

static DemoSettingsSerializer SERIALIZER;
static DemoSettingsDeserializer DESERIALIZER;

DemoProject::DemoProject(FS* fs, AsyncWebServer* server, SecurityManager* securityManager) :
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, DEMO_SETTINGS_FILE),
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, DEMO_SETTINGS_PATH, securityManager) {
  pinMode(BLINK_LED, OUTPUT);
}

DemoProject::~DemoProject() {
}

void DemoProject::begin() {
  _settingsPersistence.readFromFS();
}

void DemoProject::loop() {
  unsigned delay = MAX_DELAY / 255 * (255 - _settings.blinkSpeed);
  unsigned long currentMillis = millis();
  if (!_lastBlink || (unsigned long)(currentMillis - _lastBlink) >= delay) {
    _lastBlink = currentMillis;
    digitalWrite(BLINK_LED, !digitalRead(BLINK_LED));
  }
}
