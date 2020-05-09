#include <LightBrokerSettingsService.h>

LightBrokerSettingsService::LightBrokerSettingsService(AsyncWebServer* server,
                                                       FS* fs,
                                                       SecurityManager* securityManager) :
    _httpEndpoint(LightBrokerSettings::serialize,
                  LightBrokerSettings::deserialize,
                  this,
                  server,
                  LIGHT_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(LightBrokerSettings::serialize,
                   LightBrokerSettings::deserialize,
                   this,
                   fs,
                   LIGHT_BROKER_SETTINGS_FILE) {
}

void LightBrokerSettingsService::begin() {
  _fsPersistence.readFromFS();
}
