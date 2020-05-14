#include <LightMqttSettingsService.h>

LightMqttSettingsService::LightMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(LightMqttSettings::serialize,
                  LightMqttSettings::deserialize,
                  this,
                  server,
                  LIGHT_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(LightMqttSettings::serialize, LightMqttSettings::deserialize, this, fs, LIGHT_BROKER_SETTINGS_FILE) {
}

void LightMqttSettingsService::begin() {
  _fsPersistence.readFromFS();
}
