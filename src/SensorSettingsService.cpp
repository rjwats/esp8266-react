#include <SensorSettingsService.h>

static SensorSettingsSerializer SERIALIZER;
static SensorSettingsDeserializer DESERIALIZER;

SensorSettingsService::SensorSettingsService(AsyncWebServer* server,
                                                       FS* fs,
                                                       SecurityManager* securityManager) :
    _settingsEndpoint(&SERIALIZER,
                      &DESERIALIZER,
                      this,
                      server,
                      SENSOR_SETTINGS_PATH,
                      securityManager,
                      AuthenticationPredicates::IS_AUTHENTICATED),
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, SENSOR_SETTINGS_FILE) {
}

void SensorSettingsService::begin() {
  _settingsPersistence.readFromFS();
}
