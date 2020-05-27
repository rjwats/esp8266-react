#include <LightMqttSettingsService.h>

LightMqttSettingsService::LightMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(LightMqttSettings::read,
                  LightMqttSettings::update,
                  this,
                  server,
                  LIGHT_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(LightMqttSettings::read, LightMqttSettings::update, this, fs, LIGHT_BROKER_SETTINGS_FILE) {
}

void LightMqttSettingsService::begin() {
  _fsPersistence.readFromFS();
}
