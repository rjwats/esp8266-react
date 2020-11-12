#ifndef LedSettingsService_h
#define LedSettingsService_h

#define LED_SETTINGS_FILE "/config/ledSettings.json"
#define LED_SETTINGS_SERVICE_PATH "/rest/ledSettings"

#include <StatefulService.h>
#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <FastLED.h>

#define LED_DATA_PIN 21
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LEDS 64

#ifndef FACTORY_LED_BRIGHTNESS
#define FACTORY_LED_BRIGHTNESS 128
#endif

typedef std::function<void(const uint16_t numLeds, CRGB* _leds)> LedUpdateCallback;

// should be extended to allow number of LEDS to be modified
// should be extended to allow leds to be switched off
// could be extended to allow configuration of PIN (less likely)
// could be extended to allow configuring the strip type (less likely)
class LedSettings {
 public:
  uint8_t brightness;

  static void read(LedSettings& settings, JsonObject& root) {
    root["brightness"] = settings.brightness;
  }

  static StateUpdateResult update(JsonObject& root, LedSettings& settings) {
    settings.brightness = root["brightness"] | FACTORY_LED_BRIGHTNESS;
    return StateUpdateResult::CHANGED;
  }
};

class LedSettingsService : public StatefulService<LedSettings> {
 public:
  LedSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

  void begin();
  void update(LedUpdateCallback updateCallback);
  void showLeds();

 private:
  CRGB _leds[NUM_LEDS];
  CLEDController* _ledController;
  HttpEndpoint<LedSettings> _httpEndpoint;
  FSPersistence<LedSettings> _fsPersistence;
};

#endif  // end LedSettingsService_h
