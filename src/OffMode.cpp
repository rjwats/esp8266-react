#include <OffMode.h>

OffMode::OffMode(AsyncWebServer* server,
                 FS* fs,
                 SecurityManager* securityManager,
                 LedSettingsService* ledSettingsService,
                 PaletteSettingsService* paletteSettingsService,
                 FrequencySampler* frequencySampler) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
                       ledSettingsService,
                       paletteSettingsService,
                       frequencySampler,
                       OffModeSettings::read,
                       OffModeSettings::update,
                       OFF_MODE_ID){};

void OffMode::enable() {
  _refresh = true;
}

void OffMode::tick() {
  if (_refresh) {
    _ledSettingsService->update([&](CRGB* leds, CLEDController* ledController, const uint16_t numLeds) {
      fill_solid(leds, numLeds, CHSV(255, 0, 0));  // clear leds
      ledController->showLeds();                   // render all leds black
      _refresh = false;                            // clear refresh flag
    });
  }
}
