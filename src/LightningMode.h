#ifndef LightningMode_h
#define LightningMode_h

#include <AudioLightMode.h>
#include <JsonUtil.h>

#ifndef FACTORY_LIGHTNING_MODE_COLOR
#define FACTORY_LIGHTNING_MODE_COLOR CRGB::White
#endif

#ifndef FACTORY_LIGHTNING_MODE_FLASHES
#define FACTORY_LIGHTNING_MODE_FLASHES 8
#endif

#ifndef FACTORY_LIGHTNING_MODE_THRESHOLD
#define FACTORY_LIGHTNING_MODE_THRESHOLD 128
#endif

#ifndef FACTORY_LIGHTNING_MODE_BRIGHTNESS
#define FACTORY_LIGHTNING_MODE_BRIGHTNESS 255
#endif

#ifndef FACTORY_LIGHTNING_AUDIO_ENABLED
#define FACTORY_LIGHTNING_AUDIO_ENABLED false
#endif

#define LIGHTNING_MODE_ID "lightning"

class LightningModeSettings {
 public:
  CRGB color;
  uint8_t brightness;
  uint8_t threshold;
  uint8_t flashes;
  bool audioEnabled;
  bool includedBands[NUM_BANDS];

  static void read(LightningModeSettings& settings, JsonObject& root) {
    writeColorToJson(root, &settings.color, "color");
    writeByteToJson(root, &settings.brightness, "brightness");
    writeByteToJson(root, &settings.threshold, "threshold");
    writeByteToJson(root, &settings.flashes, "flashes");
    writeBoolToJson(root, &settings.audioEnabled, "audio_enabled");
    writeBooleanArrayToJson(root, settings.includedBands, NUM_BANDS, "included_bands");
  }

  static StateUpdateResult update(JsonObject& root, LightningModeSettings& settings) {
    updateColorFromJson(root, &settings.color, FACTORY_LIGHTNING_MODE_COLOR, "color");
    updateByteFromJson(root, &settings.brightness, FACTORY_LIGHTNING_MODE_BRIGHTNESS, "brightness");
    updateByteFromJson(root, &settings.threshold, FACTORY_LIGHTNING_MODE_THRESHOLD, "threshold");
    updateByteFromJson(root, &settings.flashes, FACTORY_LIGHTNING_MODE_FLASHES, "flashes");
    updateBoolFromJson(root, &settings.audioEnabled, FACTORY_LIGHTNING_AUDIO_ENABLED, "audio_enabled");
    updateBooleanArrayFromJson(root, settings.includedBands, NUM_BANDS, true, "included_bands");
    return StateUpdateResult::CHANGED;
  }
};

class LightningMode : public AudioLightModeImpl<LightningModeSettings> {
 private:
  enum class Status { IDLE, TRIGGERED, RUNNING };

  // State variables
  Status _status = Status::IDLE;
  uint8_t _ledstart;          // Starting location of a flash
  uint8_t _ledlen;            // Length of a flash
  uint16_t _dimmer = 1;       // State for dimming initial flash by a factor
  uint8_t _flashCounter = 0;  // Keeps track of how many flashes we have done
  uint8_t _numFlashes = 0;    // Keeps track of how many flashes we will do
  bool _refresh = true;       // For applying config updates or enabling the mode

  // Pause tracking
  unsigned long _waitStartedAt;
  uint16_t _waitDuration;

 public:
  LightningMode(AsyncWebServer* server,
                FS* fs,
                SecurityManager* securityManager,
                PaletteSettingsService* paletteSettingsService,
                FrequencySampler* frequencySampler);
  void tick(CRGB* leds, const uint16_t numLeds);
  void enable();
  void sampleComplete();
  bool isWaitTimeElapsed();
  void resetWaitTime();
};

#endif
