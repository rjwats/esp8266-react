#include <LightBrokerSettingsService.h>

static LightBrokerSettingsSerializer SERIALIZER;
static LightBrokerSettingsDeserializer DESERIALIZER;

LightBrokerSettingsService::LightBrokerSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, LIGHT_BROKER_SETTINGS_PATH, securityManager),
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, LIGHT_BROKER_SETTINGS_FILE) {
}

void LightBrokerSettingsService::begin() {
  _settingsPersistence.readFromFS();
}
