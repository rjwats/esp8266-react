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

#define COLOR_FILE_PATH "/modes/color.json"
#define COLOR_SERVICE_PATH "/rest/modes/color.json"

class ColorModeSettings {
 public:
  ColorModeSettings() {
  }

  CRGB color = FACTORY_COLOR_MODE_COLOR;
  bool audioEnabled = FACTORY_COLOR_MODE_AUDIO_ENABLED;
  bool includedBands[NUM_BANDS];

  static void read(ColorModeSettings& settings, JsonObject& root) {
    writeColorToJson(root, &settings.color);
    writeBoolToJson(root, &settings.audioEnabled, "audio_enabled");
    writeBooleanArrayToJson(root, settings.includedBands, NUM_BANDS, "included_bands");
  }

  static StateUpdateResult update(JsonObject& root, ColorModeSettings& settings) {
    updateColorFromJson(root, &settings.color, FACTORY_COLOR_MODE_COLOR);
    updateBoolFromJson(root, &settings.audioEnabled, FACTORY_COLOR_MODE_AUDIO_ENABLED, "audio_enabled");
    updateBooleanArrayFromJson(root, settings.includedBands, NUM_BANDS, "included_bands");
    return StateUpdateResult::CHANGED;
  }
};

class ColorMode : public AudioLightMode<ColorModeSettings> {
 private:
  boolean _refresh = true;

 public:
  ColorMode(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
      AudioLightMode(server,
                     fs,
                     securityManager,
                     ColorModeSettings::read,
                     ColorModeSettings::update,
                     COLOR_SERVICE_PATH,
                     COLOR_FILE_PATH) {
    addUpdateHandler([&](const String& originId) { enable(); }, false);
  };
  String getId();
  void tick();
  void enable();
};

#endif
