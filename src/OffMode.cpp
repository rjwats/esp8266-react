#include <OffMode.h>

OffMode::OffMode(AsyncWebServer* server,
                 FS* fs,
                 SecurityManager* securityManager,
                 PaletteSettingsService* paletteSettingsService,
                 FrequencySampler* frequencySampler) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
                       paletteSettingsService,
                       frequencySampler,
                       OffModeSettings::read,
                       OffModeSettings::update,
                       OFF_MODE_ID){};

void OffMode::enable() {
  _refresh = true;
}

void OffMode::tick(CRGB* leds, const uint16_t numLeds) {
  if (_refresh) {
    fill_solid(leds, numLeds, CHSV(255, 0, 0));  // clear leds
    FastLED.show();                              // render all leds black
    _refresh = false;                            // clear refresh flag
  }
}

bool OffMode::canRotate() {
  return false;
};
