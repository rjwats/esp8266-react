#ifndef PacificaMode_h
#define PacificaMode_h

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>

#define PACIFICA_MODE_ID "pacifica"

class PacificaModeSettings {
 public:
  static void read(PacificaModeSettings& settings, JsonObject& root) {
  }

  static StateUpdateResult update(JsonObject& root, PacificaModeSettings& settings) {
    return StateUpdateResult::CHANGED;
  }
};

class PacificaMode : public AudioLightModeImpl<PacificaModeSettings> {
 private:
  CRGBPalette16 pacifica_palette_1 = {0x000507,
                                      0x000409,
                                      0x00030B,
                                      0x00030D,
                                      0x000210,
                                      0x000212,
                                      0x000114,
                                      0x000117,
                                      0x000019,
                                      0x00001C,
                                      0x000026,
                                      0x000031,
                                      0x00003B,
                                      0x000046,
                                      0x14554B,
                                      0x28AA50};
  CRGBPalette16 pacifica_palette_2 = {0x000507,
                                      0x000409,
                                      0x00030B,
                                      0x00030D,
                                      0x000210,
                                      0x000212,
                                      0x000114,
                                      0x000117,
                                      0x000019,
                                      0x00001C,
                                      0x000026,
                                      0x000031,
                                      0x00003B,
                                      0x000046,
                                      0x0C5F52,
                                      0x19BE5F};
  CRGBPalette16 pacifica_palette_3 = {0x000208,
                                      0x00030E,
                                      0x000514,
                                      0x00061A,
                                      0x000820,
                                      0x000927,
                                      0x000B2D,
                                      0x000C33,
                                      0x000E39,
                                      0x001040,
                                      0x001450,
                                      0x001860,
                                      0x001C70,
                                      0x002080,
                                      0x1040BF,
                                      0x2060FF};

  boolean _refresh = true;

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
