#ifndef FireMode_h
#define FireMode_h

#include <AudioLightMode.h>
#include <JsonUtil.h>

#ifndef FACTORY_FIRE_MODE_SPARKING
#define FACTORY_FIRE_MODE_SPARKING 120
#endif

#ifndef FACTORY_FIRE_MODE_REVERSE
#define FACTORY_FIRE_MODE_REVERSE false
#endif

#ifndef FACTORY_FIRE_MODE_COOLING
#define FACTORY_FIRE_MODE_COOLING 80
#endif

#ifndef FACTORY_FIRE_MODE_PALETTE
#define FACTORY_FIRE_MODE_PALETTE "heat"
#endif

#define FIRE_MODE_ID "fire"

// audio enable
// make palette customizable
class FireModeSettings {
 public:
  Palette palette;
  bool reverse;
  uint8_t cooling;
  uint8_t sparking;
};

class FireMode : public AudioLightModeImpl<FireModeSettings> {
 private:
  bool _refresh = true;        // For applying config updates or enabling the mode
  uint8_t _heatMap[NUM_LEDS];  // Intensity map the led strip - statically allocated ATM

  void read(FireModeSettings& settings, JsonObject& root) {
    writeByteToJson(root, &settings.cooling, "cooling");
    writeByteToJson(root, &settings.sparking, "sparking");
    writeBoolToJson(root, &settings.reverse, "reverse");
    root["palette"] = settings.palette.id;
  }

  StateUpdateResult update(JsonObject& root, FireModeSettings& settings) {
    updateByteFromJson(root, &settings.sparking, FACTORY_FIRE_MODE_SPARKING, "sparking");
    updateByteFromJson(root, &settings.cooling, FACTORY_FIRE_MODE_COOLING, "cooling");
    updateBoolFromJson(root, &settings.reverse, FACTORY_FIRE_MODE_REVERSE, "reverse");

    // select the palette
    settings.palette = _paletteSettingsService->getPalette(root["palette"] | FACTORY_FIRE_MODE_PALETTE);
    // we make one change to the selected palette
    // we assert that the first color is black to create the fire effect
    settings.palette.colors[0] = 0x000000;
    return StateUpdateResult::CHANGED;
  }

 public:
  FireMode(AsyncWebServer* server,
           FS* fs,
           SecurityManager* securityManager,
           LedSettingsService* ledSettingsService,
           PaletteSettingsService* paletteSettingsService,
           FrequencySampler* frequencySampler);
  void tick();
  void enable();
  void refreshPalettes(const String& originId);
};

#endif
