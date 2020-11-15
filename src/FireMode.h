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

#define FIRE_MODE_ID "fire"

// audio enable
// make palette customizable
class FireModeSettings {
 public:
  bool reverse;
  uint8_t cooling;
  uint8_t sparking;

  static void read(FireModeSettings& settings, JsonObject& root) {
    writeByteToJson(root, &settings.cooling, "cooling");
    writeByteToJson(root, &settings.sparking, "sparking");
    writeBoolToJson(root, &settings.reverse, "reverse");
  }

  static StateUpdateResult update(JsonObject& root, FireModeSettings& settings) {
    updateByteFromJson(root, &settings.sparking, FACTORY_FIRE_MODE_SPARKING, "sparking");
    updateByteFromJson(root, &settings.cooling, FACTORY_FIRE_MODE_COOLING, "cooling");
    updateBoolFromJson(root, &settings.reverse, FACTORY_FIRE_MODE_REVERSE, "reverse");
    return StateUpdateResult::CHANGED;
  }
};

class FireMode : public AudioLightModeImpl<FireModeSettings> {
 private:
  bool _refresh = true;                               // For applying config updates or enabling the mode
  uint8_t _heatMap[NUM_LEDS];                         // Intensity map the led strip - statically allocated ATM
  CRGBPalette16 _heat = CRGBPalette16(HeatColors_p);  // The heat palette, which looks like fire
  CRGBPalette16* _firePalette = &_heat;               // The current palette we are using, set to heat ATM

 public:
  FireMode(AsyncWebServer* server,
           FS* fs,
           SecurityManager* securityManager,
           LedSettingsService* ledSettingsService,
           FrequencySampler* frequencySampler);
  void tick();
  void enable();
};

#endif
