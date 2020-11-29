#ifndef ConfettiMode_h
#define ConfettiMode_h

#include <AudioLightMode.h>
#include <JsonUtil.h>

#ifndef FACTORY_CONFETTI_MODE_MAX_CHANGES
#define FACTORY_CONFETTI_MODE_MAX_CHANGES 24
#endif

#ifndef FACTORY_CONFETTI_MODE_BRIGHTNESS
#define FACTORY_CONFETTI_MODE_BRIGHTNESS 255
#endif

#ifndef FACTORY_CONFETTI_MODE_DELAY
#define FACTORY_CONFETTI_MODE_DELAY 5
#endif

#ifndef FACTORY_CONFETTI_MODE_PALETTE1
#define FACTORY_CONFETTI_MODE_PALETTE1 "Ocean"
#endif

#ifndef FACTORY_CONFETTI_MODE_PALETTE2
#define FACTORY_CONFETTI_MODE_PALETTE2 "Lava"
#endif

#ifndef FACTORY_CONFETTI_MODE_PALETTE3
#define FACTORY_CONFETTI_MODE_PALETTE3 "Forest"
#endif

#define CONFETTI_MODE_ID "confetti"

// could make palettes configurable, pick from a list perhaps...
class ConfettiModeSettings {
 public:
  uint8_t maxChanges;  // number of changes per cycle
  uint8_t brightness;  // Brightness of a sequence. Remember, max_brightnessght is the overall limiter.
  uint8_t delay;       // We don't need much delay (if any)

  Palette palette1;
  Palette palette2;
  Palette palette3;
};

class ConfettiMode : public AudioLightModeImpl<ConfettiModeSettings> {
 private:
  // Variables used by the sequences.
  uint8_t _fade = 8;    // How quickly does it fade? Lower = slower fade rate.
  int _hue = 50;        // Starting hue.
  uint8_t _inc = 1;     // Incremental value for rotating hues
  int _hueDelta = 255;  // Range of random #'s to use for hue

  CRGBPalette16 _currentPalette;              // Current primary target
  CRGBPalette16 _targetPalette;               // Pallette we are blending to
  TBlendType _currentBlending = LINEARBLEND;  // NOBLEND or LINEARBLEND

  bool _refresh = true;  // For applying config updates or enabling the mode

  void read(ConfettiModeSettings& settings, JsonObject& root) {
    writeByteToJson(root, &settings.maxChanges, "max_changes");
    writeByteToJson(root, &settings.brightness, "brightness");
    writeByteToJson(root, &settings.delay, "delay");

    root["palette1"] = settings.palette1.name;
    root["palette2"] = settings.palette2.name;
    root["palette3"] = settings.palette3.name;
  }

  StateUpdateResult update(JsonObject& root, ConfettiModeSettings& settings) {
    updateByteFromJson(root, &settings.maxChanges, FACTORY_CONFETTI_MODE_MAX_CHANGES, "max_changes");
    updateByteFromJson(root, &settings.brightness, FACTORY_CONFETTI_MODE_BRIGHTNESS, "brightness");
    updateByteFromJson(root, &settings.delay, FACTORY_CONFETTI_MODE_DELAY, "delay");

    settings.palette1 = _paletteSettingsService->getPalette(root["palette1"] | FACTORY_CONFETTI_MODE_PALETTE1);
    settings.palette2 = _paletteSettingsService->getPalette(root["palette2"] | FACTORY_CONFETTI_MODE_PALETTE2);
    settings.palette3 = _paletteSettingsService->getPalette(root["palette3"] | FACTORY_CONFETTI_MODE_PALETTE3);
    return StateUpdateResult::CHANGED;
  }

 public:
  ConfettiMode(AsyncWebServer* server,
               FS* fs,
               SecurityManager* securityManager,
               PaletteSettingsService* paletteSettingsService,
               FrequencySampler* frequencySampler);
  void tick(CRGB* leds, const uint16_t numLeds);
  void enable();
};

#endif
