#ifndef LedSettingsService_h
#define LedSettingsService_h

#define LED_SETTINGS_FILE "/config/ledSettings.json"
#define LED_SETTINGS_SERVICE_PATH "/rest/ledSettings"

#include <StatefulService.h>
#include <HttpEndpoint.h>
#include <FSPersistence.h>

#ifndef FACTORY_LED_MAX_POWER_MILLIWATTS
#define FACTORY_LED_MAX_POWER_MILLIWATTS 0
#endif

#ifndef FACTORY_LED_SMOOTHING_FACTOR
#define FACTORY_LED_SMOOTHING_FACTOR 0.15
#endif

#ifndef FACTORY_LED_DEAD_ZONE
#define FACTORY_LED_DEAD_ZONE 700
#endif

class LedSettings {
 public:
  uint32_t maxPowerMilliwatts;
  uint16_t deadZone;
  float smoothingFactor;

  static void read(LedSettings& settings, JsonObject& root) {
    root["max_power_milliwatts"] = settings.maxPowerMilliwatts;
    root["dead_zone"] = settings.deadZone;
    root["smoothing_factor"] = settings.smoothingFactor;
  }

  static StateUpdateResult update(JsonObject& root, LedSettings& settings) {
    settings.maxPowerMilliwatts = root["max_power_milliwatts"] | FACTORY_LED_MAX_POWER_MILLIWATTS;
    settings.deadZone = root["dead_zone"] | FACTORY_LED_DEAD_ZONE;
    settings.smoothingFactor = root["smoothing_factor"] | FACTORY_LED_SMOOTHING_FACTOR;
    return StateUpdateResult::CHANGED;
  }
};

class LedSettingsService : public StatefulService<LedSettings> {
 public:
  LedSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();

 private:
  HttpEndpoint<LedSettings> _httpEndpoint;
  FSPersistence<LedSettings> _fsPersistence;
};

#endif  // end LedSettingsService_h
