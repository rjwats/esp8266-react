#ifndef COLOR_MODE_H
#define COLOR_MODE_H

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>

#ifndef FACTORY_COLOR_MODE_COLOR
#define FACTORY_COLOR_MODE_COLOR CRGB::White
#endif

#ifndef FACTORY_COLOR_MODE_AUDIO_ENABLED
#define FACTORY_COLOR_MODE_AUDIO_ENABLED false
#endif

#ifndef FACTORY_COLOR_MODE_BRIGHTNESS
#define FACTORY_COLOR_MODE_BRIGHTNESS 128
#endif

#define COLOR_MODE_ID "color"

class ColorModeSettings {
 public:
  ColorModeSettings() {
  }

  CRGB color;
  uint8_t brightness;
  bool audioEnabled;
  bool includedBands[NUM_BANDS];

  static void read(ColorModeSettings& settings, JsonObject& root) {
    writeColorToJson(root, &settings.color);
    writeByteToJson(root, &settings.brightness, "brightness");
    writeBoolToJson(root, &settings.audioEnabled, "audio_enabled");
    writeBooleanArrayToJson(root, settings.includedBands, NUM_BANDS, "included_bands");
  }

  static StateUpdateResult update(JsonObject& root, ColorModeSettings& settings) {
    updateColorFromJson(root, &settings.color, FACTORY_COLOR_MODE_COLOR);
    updateByteFromJson(root, &settings.brightness, FACTORY_COLOR_MODE_BRIGHTNESS, "brightness");
    updateBoolFromJson(root, &settings.audioEnabled, FACTORY_COLOR_MODE_AUDIO_ENABLED, "audio_enabled");
    updateBooleanArrayFromJson(root, settings.includedBands, NUM_BANDS, "included_bands");
    return StateUpdateResult::CHANGED;
  }
};

class ColorMode : public AudioLightModeImpl<ColorModeSettings> {
 private:
  boolean _refresh = true;

 public:
  ColorMode(AsyncWebServer* server,
            FS* fs,
            SecurityManager* securityManager,
            LedSettingsService* ledSettingsService,
            FrequencySampler* frequencySampler);
  const String& getId();
  void tick();
  void enable();
};

#endif
