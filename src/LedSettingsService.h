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

#ifndef FACTORY_LED_BRIGHTNESS
#define FACTORY_LED_BRIGHTNESS 128
#endif

#ifndef FACTORY_LED_SMOOTHING_FACTOR
#define FACTORY_LED_SMOOTHING_FACTOR 0.15
#endif

typedef std::function<void(CRGB* _leds, CLEDController* _ledController, const uint16_t numLeds)> LedUpdateCallback;

// should be extended to allow number of LEDS to be modified
// should be extended to allow leds to be switched off
// could be extended to allow configuration of PIN (less likely)
// could be extended to allow configuring the strip type (less likely)
class LedSettings {
 public:
  uint8_t brightness;
  float smoothingFactor;

  static void read(LedSettings& settings, JsonObject& root) {
    root["brightness"] = settings.brightness;
    root["smoothing_factor"] = settings.smoothingFactor;
  }

  static StateUpdateResult update(JsonObject& root, LedSettings& settings) {
    settings.brightness = root["brightness"] | FACTORY_LED_BRIGHTNESS;
    settings.smoothingFactor = root["smoothing_factor"] | FACTORY_LED_SMOOTHING_FACTOR;
    return StateUpdateResult::CHANGED;
  }
};

class LedSettingsService : public StatefulService<LedSettings>, public FrequencySamplerSettings {
 public:
  LedSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  float getSmoothingFactor();
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
