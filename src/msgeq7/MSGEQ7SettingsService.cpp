#include <msgeq7/MSGEQ7SettingsService.h>

MSGEQ7SettingsService::MSGEQ7SettingsService(AsyncWebServer* server, FS* fs) : SettingsService(server, fs, MSGEQ7_SETTINGS_SERVICE_PATH, MSGEQ7_SETTINGS_FILE) {
}

MSGEQ7SettingsService::~MSGEQ7SettingsService() {}

void MSGEQ7SettingsService::begin() {
  Serial.println("Beginning MSGEQ07 Now");
}

void MSGEQ7SettingsService::loop() {
  unsigned long currentMillis = millis();
  unsigned long sampleElapsed = (unsigned long)(currentMillis - _lastSampledAt);
  if (sampleElapsed >= MSGEQ7_SAMPLE_FREQUENCY_MS){
    _lastSampledAt = currentMillis;
    sampleNow();
  }
}

void MSGEQ7SettingsService::readFromJsonObject(JsonObject& root){

}

void MSGEQ7SettingsService::writeToJsonObject(JsonObject& root){

}

void MSGEQ7SettingsService::sampleNow() {
  Serial.println("Sampling Now");
}