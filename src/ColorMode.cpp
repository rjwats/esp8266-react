#include <ColorMode.h>

ColorMode::ColorMode(AsyncWebServer* server,
                     FS* fs,
                     SecurityManager* securityManager,
                     LedSettingsService* ledSettingsService,
                     FrequencySampler* frequencySampler) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
                       ledSettingsService,
                       frequencySampler,
                       ColorModeSettings::read,
                       ColorModeSettings::update,
                       COLOR_MODE_ID) {
  addUpdateHandler([&](const String& originId) { enable(); }, false);
};

void ColorMode::enable() {
  _refresh = true;
}

void ColorMode::tick() {
  if (_refresh || _state.audioEnabled) {
    _ledSettingsService->update([&](CRGB* leds, CLEDController* ledController, const uint16_t numLeds) {
      FrequencyData* frequencyData = _frequencySampler->getFrequencyData();
      fill_solid(leds, numLeds, _state.color);
      ledController->showLeds(_state.audioEnabled
                                  ? frequencyData->calculateEnergyFloat(_state.includedBands) * _state.brightness
                                  : _state.brightness);
    });
    _refresh = false;
  }
}
