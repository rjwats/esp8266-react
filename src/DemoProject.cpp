#include <DemoProject.h>

static DemoSettingsSerializer SERIALIZER;
static DemoSettingsDeserializer DESERIALIZER;

DemoProject::DemoProject(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, DEMO_SETTINGS_PATH, securityManager),
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, DEMO_SETTINGS_FILE) {
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
