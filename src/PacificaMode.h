#ifndef PacificaMode_h
#define PacificaMode_h

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>

#define PACIFICA_MODE_PALETTE1 "Pacifica 1"
#define PACIFICA_MODE_PALETTE2 "Pacifica 2"
#define PACIFICA_MODE_PALETTE3 "Pacifica 3"

#define PACIFICA_MODE_ID "pacifica"

class PacificaModeSettings {
 public:
  Palette palette1;
  Palette palette2;
  Palette palette3;
};

class PacificaMode : public AudioLightModeImpl<PacificaModeSettings> {
 private:
  boolean _refresh = true;

  void read(PacificaModeSettings& settings, JsonObject& root) {
    root["palette1"] = settings.palette1.id;
    root["palette2"] = settings.palette2.id;
    root["palette3"] = settings.palette3.id;
  }

  StateUpdateResult update(JsonObject& root, PacificaModeSettings& settings) {
    settings.palette1 = _paletteSettingsService->getPalette(root["palette1"] | PACIFICA_MODE_PALETTE1);
    settings.palette2 = _paletteSettingsService->getPalette(root["palette2"] | PACIFICA_MODE_PALETTE2);
    settings.palette3 = _paletteSettingsService->getPalette(root["palette3"] | PACIFICA_MODE_PALETTE3);
    return StateUpdateResult::CHANGED;
  }

  void pacifica_one_layer(CRGB* leds,
                          const uint16_t numLeds,
                          CRGBPalette16& p,
                          uint16_t cistart,
                          uint16_t wavescale,
                          uint8_t bri,
                          uint16_t ioff);
  void pacifica_deepen_colors(CRGB* leds, const uint16_t numLeds);
  void pacifica_add_whitecaps(CRGB* leds, const uint16_t numLeds);

 public:
  PacificaMode(AsyncWebServer* server,
               FS* fs,
               SecurityManager* securityManager,
               LedSettingsService* ledSettingsService,
               PaletteSettingsService* paletteSettingsService,
               FrequencySampler* frequencySampler);
  void tick();
  void enable();
};

#endif
