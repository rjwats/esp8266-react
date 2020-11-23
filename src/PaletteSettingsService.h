#ifndef PaletteSettingsService_h
#define PaletteSettingsService_h

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>
#include <map>

#define PALETTE_SETTINGS_FILE "/config/paletteSettings.json"
#define PALETTE_SETTINGS_SERVICE_PATH "/rest/paletteSettings"

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
  CRGBPalette16 colors;

  // the default palette - rainbow
  Palette() : id("Rainbow"), colors(RainbowColors_p) {
  }

  // custom palettes
  Palette(String id, CRGBPalette16 colors) : id(id), colors(colors) {
  }
};

class PaletteSettings {
 public:
  std::list<Palette> palettes;

  static void read(PaletteSettings& settings, JsonObject& root) {
    JsonArray palettes = root.createNestedArray("palettes");
    for (Palette palette : settings.palettes) {
      JsonObject paletteRoot = palettes.createNestedObject();
      paletteRoot["id"] = palette.id;
      writePaletteToJson(paletteRoot, &palette.colors);
    }
  }

  static StateUpdateResult update(JsonObject& root, PaletteSettings& settings) {
    settings.palettes.clear();
    if (root["palettes"].is<JsonArray>()) {
      for (JsonObject paletteRoot : root["palettes"].as<JsonArray>()) {
        String id = paletteRoot["id"];
        if (!id.isEmpty()) {
          Palette palette = Palette();
          palette.id = id;
          updatePaletteFromJson(paletteRoot, &palette.colors, CRGB::Black);
          settings.palettes.push_back(palette);
        }
      }
    } else {
      settings.palettes.push_back(Palette());
      settings.palettes.push_back(Palette("Party", PartyColors_p));
      settings.palettes.push_back(Palette("Heat", HeatColors_p));
      settings.palettes.push_back(Palette("Rainbow Stripe", RainbowStripeColors_p));
      settings.palettes.push_back(Palette("Cloud", CloudColors_p));
      settings.palettes.push_back(Palette("Lava", LavaColors_p));
      settings.palettes.push_back(Palette("Ocean", OceanColors_p));
      settings.palettes.push_back(Palette("Forest", ForestColors_p));
      settings.palettes.push_back(Palette("Pacifica 1", PacificaColors1_p));
      settings.palettes.push_back(Palette("Pacifica 2", PacificaColors2_p));
      settings.palettes.push_back(Palette("Pacifica 3", PacificaColors3_p));
    }
    return StateUpdateResult::CHANGED;
  }
};

/**
 * May be extended to support a palette editor in the UI.
 */
class PaletteSettingsService : public StatefulService<PaletteSettings> {
 private:
  HttpEndpoint<PaletteSettings> _httpEndpoint;
  FSPersistence<PaletteSettings> _fsPersistence;

 public:
  PaletteSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
      _httpEndpoint(PaletteSettings::read,
                    PaletteSettings::update,
                    this,
                    server,
                    PALETTE_SETTINGS_SERVICE_PATH,
                    securityManager,
                    AuthenticationPredicates::IS_AUTHENTICATED,
                    10240),
      _fsPersistence(PaletteSettings::read, PaletteSettings::update, this, fs, PALETTE_SETTINGS_FILE, 10240) {
  }

  void begin() {
    _fsPersistence.readFromFS();
  }

  void refreshPalette(Palette& palette) {
    palette = getPalette(palette.id);
  }

  Palette getPalette(const String& paletteId) {
    for (Palette palette : _state.palettes) {
      if (palette.id == paletteId) {
        return palette;
      }
    }
    return Palette();
  }
};

#endif
