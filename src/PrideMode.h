#ifndef PrideMode_h
#define PrideMode_h

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>

#define PRIDE_MODE_ID "pride"

#ifndef FACTORY_PRIDE_MODE_BRIGHTNESS_MULTIPLIER_MIN
#define FACTORY_PRIDE_MODE_BRIGHTNESS_MULTIPLIER_MIN 23
#endif

#ifndef FACTORY_PRIDE_MODE_BRIGHTNESS_MULTIPLIER_MAX
#define FACTORY_PRIDE_MODE_BRIGHTNESS_MULTIPLIER_MAX 60
#endif

#ifndef FACTORY_PRIDE_MODE_BRIGHTNESS_BPM
#define FACTORY_PRIDE_MODE_BRIGHTNESS_BPM 147
#endif

#ifndef FACTORY_PRIDE_MODE_HUE_BPM
#define FACTORY_PRIDE_MODE_HUE_BPM 113
#endif

#ifndef FACTORY_PRIDE_MODE_HUDE_DELTA_MIN
#define FACTORY_PRIDE_MODE_HUDE_DELTA_MIN 1
#endif

#ifndef FACTORY_PRIDE_MODE_HUDE_DELTA_MAX
#define FACTORY_PRIDE_MODE_HUDE_DELTA_MAX 3000
#endif

class PrideModeSettings {
 public:
  uint8_t brightnessBpm;
  uint8_t brightnessFreqMin;
  uint8_t brightnessFreqMax;

  uint8_t hueBpm;
  uint16_t hueDeltaMin;
  uint16_t hueDeltaMax;

  static void read(PrideModeSettings& settings, JsonObject& root) {
    writeByteToJson(root, &settings.brightnessBpm, "brightness_bpm");
    writeByteToJson(root, &settings.brightnessFreqMin, "brightness_freq_min");
    writeByteToJson(root, &settings.brightnessFreqMax, "brightness_freq_max");
    writeByteToJson(root, &settings.hueBpm, "hue_bpm");
    root["hue_delta_min"] = settings.hueDeltaMin;
    root["hue_delta_max"] = settings.hueDeltaMax;
  }

  static StateUpdateResult update(JsonObject& root, PrideModeSettings& settings) {
    updateByteFromJson(root, &settings.brightnessBpm, FACTORY_PRIDE_MODE_BRIGHTNESS_BPM, "brightness_bpm");
    updateByteFromJson(
        root, &settings.brightnessFreqMin, FACTORY_PRIDE_MODE_BRIGHTNESS_MULTIPLIER_MIN, "brightness_freq_min");
    updateByteFromJson(
        root, &settings.brightnessFreqMax, FACTORY_PRIDE_MODE_BRIGHTNESS_MULTIPLIER_MAX, "brightness_freq_max");
    updateByteFromJson(root, &settings.hueBpm, FACTORY_PRIDE_MODE_HUE_BPM, "hue_bpm");
    settings.hueDeltaMin = root["hue_delta_min"] | FACTORY_PRIDE_MODE_HUDE_DELTA_MIN;
    settings.hueDeltaMax = root["hue_delta_max"] | FACTORY_PRIDE_MODE_HUDE_DELTA_MAX;
    return StateUpdateResult::CHANGED;
  }
};

class PrideMode : public AudioLightModeImpl<PrideModeSettings> {
 private:
  boolean _refresh = true;

 public:
  PrideMode(AsyncWebServer* server,
            FS* fs,
            SecurityManager* securityManager,
            PaletteSettingsService* paletteSettingsService,
            FrequencySampler* frequencySampler);
  void tick(CRGB* leds, const uint16_t numLeds);
  void enable();
};

#endif
