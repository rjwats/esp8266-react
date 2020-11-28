#ifndef OffMode_h
#define OffMode_h

#include <AudioLightMode.h>
#include <JsonUtil.h>

#define OFF_MODE_ID "off"

class OffModeSettings {
 public:
  static void read(OffModeSettings& settings, JsonObject& root) {
  }

  static StateUpdateResult update(JsonObject& root, OffModeSettings& settings) {
    return StateUpdateResult::CHANGED;
  }
};

class OffMode : public AudioLightModeImpl<OffModeSettings> {
 private:
  bool _refresh = true;  // For applying config updates or enabling the mode
 public:
  OffMode(AsyncWebServer* server,
          FS* fs,
          SecurityManager* securityManager,
          PaletteSettingsService* paletteSettingsService,
          FrequencySampler* frequencySampler);
  void tick(CRGB* leds, const uint16_t numLeds);
  void enable();
  bool canRotate();
};

#endif
