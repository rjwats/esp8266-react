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
}

void LedSettingsService::begin() {
  _fsPersistence.readFromFS();
}
