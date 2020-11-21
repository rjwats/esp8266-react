#ifndef PaletteManager_h
#define PaletteManager_h

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>
#include <map>

const TProgmemRGBPalette16 PacificaColors1_p FL_PROGMEM = {0x000507,
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

const TProgmemRGBPalette16 PacificaColors2_p FL_PROGMEM = {0x000507,
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

const TProgmemRGBPalette16 PacificaColors3_p FL_PROGMEM = {0x000208,
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

class Palette {
 public:
  String id;
  CRGBPalette16 palette;

  // the default palette - rainbow
  Palette() : id("rainbow"), palette(RainbowColors_p) {
  }

  // custom palettes
  Palette(String id, CRGBPalette16 palette) : id(id), palette(palette) {
  }
};

/**
 * May be extended to support a palette editor in the UI.
 */
class PaletteManager {
 private:
  std::list<Palette> palettes;

 public:
  PaletteManager() {
    palettes.push_back(Palette());
    palettes.push_back(Palette("partycolors", PartyColors_p));
    palettes.push_back(Palette("heat", HeatColors_p));
    palettes.push_back(Palette("rainbowstripe", RainbowStripeColors_p));
    palettes.push_back(Palette("cloud", CloudColors_p));
    palettes.push_back(Palette("lava", LavaColors_p));
    palettes.push_back(Palette("ocean", OceanColors_p));
    palettes.push_back(Palette("forest", ForestColors_p));
    palettes.push_back(Palette("pacifica1", PacificaColors1_p));
    palettes.push_back(Palette("pacifica2", PacificaColors2_p));
    palettes.push_back(Palette("pacifica3", PacificaColors3_p));
  }

  Palette getPalette(const String& paletteId) {
    for (const Palette& palette : palettes) {
      if (palette.id == paletteId) {
        return palette;
      }
    }
    return Palette();
  }
};

#endif
