#include <ColorMode.h>

ColorMode::ColorMode(AsyncWebServer* server,
                     FS* fs,
                     SecurityManager* securityManager,
                     PaletteSettingsService* paletteSettingsService,
                     FrequencySampler* frequencySampler) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
                       paletteSettingsService,
                       frequencySampler,
                       ColorModeSettings::read,
                       ColorModeSettings::update,
                       COLOR_MODE_ID) {
  addUpdateHandler([&](const String& originId) { enable(); }, false);
};

void ColorMode::enable() {
  _refresh = true;
}

void ColorMode::tick(CRGB* leds, const uint16_t numLeds) {
  if (_refresh || _state.audioEnabled) {
    FrequencyData* frequencyData = _frequencySampler->getFrequencyData();
    fill_solid(leds, numLeds, _state.color);
    FastLED.show(_state.audioEnabled ? frequencyData->calculateEnergyFloat(_state.includedBands) * _state.brightness
                                     : _state.brightness);
    _refresh = false;
  }
}
