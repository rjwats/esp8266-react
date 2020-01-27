#include <DemoProject.h>

DemoProject::DemoProject(AsyncWebServer* server, FS* fs, ESP8266React* esp8266React) :
    AdminSettingsService(server, fs, esp8266React->getSecurityManager(), DEMO_SETTINGS_PATH, DEMO_SETTINGS_FILE),
    _esp8266React(esp8266React) {
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
  if (!_lastEcho || (unsigned long)(currentMillis - _lastEcho) >= ECHO_CFG_DELAY) {
    _lastEcho = currentMillis;

    // round-trip the settings
    String config;
    _esp8266React->getSecuritySettingsService()->fetchAsString(config);
    Serial.println(config);
    _esp8266React->getSecuritySettingsService()->updateFromString(config);

    // grab the settings object from one of the services
    WiFiSettings settings = _esp8266React->getWiFiSettingsService()->fetch();
    Serial.print("The ssid is:");
    Serial.println(settings.ssid);

    // toggle the observation on and off
    if (_updateHandler) {
      Serial.println("No longer observing OTA updates...");
      _esp8266React->getOTASettingsService()->removeUpdateHandler(_updateHandler);
      _updateHandler = 0;
    } else {
      Serial.println("Now observing OTA updates...");
      _updateHandler =
          _esp8266React->getOTASettingsService()->addUpdateHandler(std::bind(&DemoProject::onOTASettingsUpdated, this));
    }
  }
}

void DemoProject::readFromJsonObject(JsonObject& root) {
  _settings.blinkSpeed = root["blink_speed"] | DEFAULT_BLINK_SPEED;
}

void DemoProject::writeToJsonObject(JsonObject& root) {
  // connection settings
  root["blink_speed"] = _settings.blinkSpeed;
}

void DemoProject::onOTASettingsUpdated() {
  WiFiSettings wifiSettings = _esp8266React->getWiFiSettingsService()->fetch();

  Serial.print("WiFiSettings updated, hostname is now:");
  Serial.println(wifiSettings.hostname);
}
