#include <RainbowMode.h>

RainbowMode::RainbowMode(AsyncWebServer* server,
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
                       RainbowModeSettings::read,
                       RainbowModeSettings::update,
                       RAINBOW_MODE_ID) {
  _ledsPerBand = NUM_LEDS / NUM_BANDS;
  _remainingLeds = NUM_LEDS % NUM_BANDS;
};

void RainbowMode::enable() {
  _lastFrameMicros = micros();
}

void RainbowMode::tick() {
  _ledSettingsService->update([&](CRGB* leds, const uint16_t numLeds) {
    FrequencyData* frequencyData = _frequencySampler->getFrequencyData();

    // rotate hue in time based manner
    if (_state.rotateSpeed > 0) {
      unsigned long rotateDelayMicros = 1000000 / _state.rotateSpeed;
      unsigned long currentMicros = micros();
      unsigned long microsElapsed = (unsigned long)(currentMicros - _lastFrameMicros);
      if (microsElapsed >= rotateDelayMicros) {
        _lastFrameMicros = currentMicros;
        _initialhue++;
      }
    }

    // fill the rainbow
    fill_rainbow(leds, numLeds, _initialhue, _state.hueDelta);

    // fade each segment if audio-enabled
    if (_state.audioEnabled) {
      CRGB* startLed = leds;
      for (uint8_t i = 0; i < NUM_BANDS; i++) {
        uint16_t numLeds = _ledsPerBand + (i == NUM_BANDS - 1 ? _remainingLeds : 0);
        fadeToBlackBy(startLed, numLeds, 255 - map(frequencyData->bands[i], 0, ADC_MAX_VALUE, 0, 255));
        startLed += _ledsPerBand;
      }
    }



    // update the leds
    FastLED.show(_state.brightness);
  });
}
