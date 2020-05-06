#include <LightBrokerSettingsService.h>

LightBrokerSettingsService::LightBrokerSettingsService(AsyncWebServer* server,
                                                       FS* fs,
                                                       SecurityManager* securityManager) :
    _settingsEndpoint(LightBrokerSettings::serialize,
                      LightBrokerSettings::deserialize,
                      this,
                      server,
                      LIGHT_BROKER_SETTINGS_PATH,
                      securityManager,
                      AuthenticationPredicates::IS_AUTHENTICATED),
    _settingsPersistence(LightBrokerSettings::serialize,
                         LightBrokerSettings::deserialize,
                         this,
                         fs,
                         LIGHT_BROKER_SETTINGS_FILE) {
}

void LightBrokerSettingsService::begin() {
  _settingsPersistence.readFromFS();
}
