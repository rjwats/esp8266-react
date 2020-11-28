#include <LedSettingsService.h>

LedSettingsService::LedSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(LedSettings::read,
                  LedSettings::update,
                  this,
                  server,
                  LED_SETTINGS_SERVICE_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(LedSettings::read, LedSettings::update, this, fs, LED_SETTINGS_FILE) {
  _ledController = &FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS);
  addUpdateHandler([&](const String& originId) { configureLeds(); }, false);
}

void LedSettingsService::begin() {
  _fsPersistence.readFromFS();
}

void LedSettingsService::update(LedUpdateCallback updateCallback) {
  updateCallback(_leds, _ledController, NUM_LEDS);
}

void LedSettingsService::configureLeds() {
  // zero means "unlimited"
  FastLED.setMaxPowerInMilliWatts(_state.maxPowerMilliwatts == 0 ? 0xFFFFFFFF : _state.maxPowerMilliwatts);
}

float LedSettingsService::getSmoothingFactor() {
  return _state.smoothingFactor;
}

uint16_t LedSettingsService::getDeadZone() {
  return _state.deadZone;
}
