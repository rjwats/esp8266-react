#include <LightSettingsService.h>

static LightSettingsSerializer SERIALIZER;
static LightSettingsDeserializer DESERIALIZER;

LightSettingsService::LightSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, LIGHT_SETTINGS_PATH, securityManager),
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, LIGHT_SETTINGS_FILE) {
}

void LightSettingsService::begin() {
  _settingsPersistence.readFromFS();
}
