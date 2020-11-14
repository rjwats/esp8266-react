#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <AudioLightMode.h>
#include <JsonUtil.h>

#ifndef FACTORY_RAINBOW_MODE_HUE_DELTA
#define FACTORY_RAINBOW_MODE_HUE_DELTA 250
#endif

#ifndef FACTORY_RAINBOW_MODE_BRIGHTNESS
#define FACTORY_RAINBOW_MODE_BRIGHTNESS 128
#endif

#ifndef FACTORY_RAINBOW_MODE_ROTATE_SPEED
#define FACTORY_RAINBOW_MODE_ROTATE_SPEED 32
#endif

#ifndef FACTORY_RAINBOW_MODE_AUDIO_ENABLED
#define FACTORY_RAINBOW_MODE_AUDIO_ENABLED false
#endif

#define RAINBOW_MODE_ID "rainbow"

class RainbowModeSettings {
 public:
  uint8_t brightness;
  uint8_t rotateSpeed;
  bool audioEnabled;
  uint8_t hueDelta;

  static void read(RainbowModeSettings& settings, JsonObject& root) {
    writeByteToJson(root, &settings.brightness, "brightness");
    writeByteToJson(root, &settings.rotateSpeed, "rotate_speed");
    writeBoolToJson(root, &settings.audioEnabled, "audio_enabled");
    writeByteToJson(root, &settings.hueDelta, "hue_delta");
  }

  static StateUpdateResult update(JsonObject& root, RainbowModeSettings& settings) {
    updateByteFromJson(root, &settings.brightness, FACTORY_RAINBOW_MODE_BRIGHTNESS, "brightness");
    updateByteFromJson(root, &settings.rotateSpeed, FACTORY_RAINBOW_MODE_ROTATE_SPEED, "rotate_speed");
    updateBoolFromJson(root, &settings.audioEnabled, FACTORY_RAINBOW_MODE_AUDIO_ENABLED, "audio_enabled");
    updateByteFromJson(root, &settings.hueDelta, FACTORY_RAINBOW_MODE_HUE_DELTA, "hue_delta");
    return StateUpdateResult::CHANGED;
  }
};

class RainbowMode : public AudioLightModeImpl<RainbowModeSettings> {
 private:
  // various state and settings for rainbow mode
  uint16_t _ledsPerBand;
  uint16_t _remainingLeds;

  // delay state for rotation
  unsigned long _lastFrameMicros = 0;

  uint8_t _initialhue = 0;
  boolean _refresh = true;

 public:
  RainbowMode(AsyncWebServer* server,
              FS* fs,
              SecurityManager* securityManager,
              LedSettingsService* ledSettingsService,
              FrequencySampler* frequencySampler);
  void tick();
  void enable();
};

#endif
