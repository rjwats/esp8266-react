#ifndef PrideMode_h
#define PrideMode_h

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>

#define PRIDE_MODE_ID "pride"

class PrideModeSettings {
 public:
  static void read(PrideModeSettings& settings, JsonObject& root) {
  }

  static StateUpdateResult update(JsonObject& root, PrideModeSettings& settings) {
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
               LedSettingsService* ledSettingsService,
               FrequencySampler* frequencySampler);
  void tick();
  void enable();
};

#endif
