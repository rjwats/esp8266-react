#ifndef LedSettingsService_h
#define LedSettingsService_h

#define LED_SETTINGS_FILE "/config/ledSettings.json"
#define LED_SETTINGS_SERVICE_PATH "/rest/ledSettings"

#include <FrequencySampler.h>
#include <StatefulService.h>
#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <FastLED.h>

#define LED_DATA_PIN 21
#define COLOR_ORDER RGB
#define LED_TYPE WS2811
#define NUM_LEDS 50

/*
#define LED_DATA_PIN 21
#define COLOR_ORDER GRB  // GBR
#define LED_TYPE WS2812B
#define NUM_LEDS 9
*/

#ifndef FACTORY_LED_MAX_POWER_MILLIWATTS
#define FACTORY_LED_MAX_POWER_MILLIWATTS 0
#endif

#ifndef FACTORY_LED_SMOOTHING_FACTOR
#define FACTORY_LED_SMOOTHING_FACTOR 0.15
#endif

#ifndef FACTORY_LED_DEAD_ZONE
#define FACTORY_LED_DEAD_ZONE 700
#endif

typedef std::function<void(CRGB* _leds, const uint16_t numLeds)> LedUpdateCallback;

/**
 * May be extended to support configuration of number of leds per strip.
 *
 * This is slightly tricky in the current state of the code as some effects rely on NUM_LEDS constant to set up data
 * arrays.
 *
 * - Effects could be made to dynamically generate their data arrays and alter them when required
 * - Effects could be destroyed and re-configured via a factory pattern which may allow for greater flexibility.
 * - The callback approach could be abandoned in favor of direct access (static variable or singleton)
 *
 * The latter approach would mean effects must be destructor friendly but it would be much more memory efficent that way
 * too because only the "active" effect needs be memory resident.
 */
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

class LedSettingsService : public StatefulService<LedSettings>, public FrequencySamplerSettings {
 public:
  LedSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  float getSmoothingFactor();
  uint16_t getDeadZone();

  void begin();
  void update(LedUpdateCallback updateCallback);

 private:
  CRGB _leds[NUM_LEDS];
  CLEDController* _ledController;
  HttpEndpoint<LedSettings> _httpEndpoint;
  FSPersistence<LedSettings> _fsPersistence;
  void configureLeds();
};

#endif  // end LedSettingsService_h
