#include <ConfettiMode.h>

ConfettiMode::ConfettiMode(AsyncWebServer* server,
                           FS* fs,
                           SecurityManager* securityManager,
                           LedSettingsService* ledSettingsService,
                           FrequencySampler* frequencySampler) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
                       ledSettingsService,
                       frequencySampler,
                       ConfettiModeSettings::read,
                       ConfettiModeSettings::update,
                       CONFETTI_MODE_ID) {
  addUpdateHandler([&](const String& originId) { enable(); }, false);
};

void ConfettiMode::tick() {
  _ledSettingsService->update([&](CRGB* leds, CLEDController* ledController, const uint16_t numLeds) {
    if (_refresh) {
      fill_solid(leds, numLeds, CHSV(255, 0, 0));
      ledController->showLeds();
      _refresh = false;
    }

    EVERY_N_MILLISECONDS(100) {
      nblendPaletteTowardPalette(_currentPalette, _targetPalette, _state.maxChanges);
    }
    uint8_t secondHand = (millis() / 1000) % 15;
    static uint8_t lastSecond = 99;
    if (lastSecond != secondHand) {
      lastSecond = secondHand;
      switch (secondHand) {
        case 0:
          _targetPalette = OceanColors_p;
          _inc = 1;
          _hue = 192;
          _fade = 2;
          _hueDelta = 255;
          break;
        case 5:
          _targetPalette = LavaColors_p;
          _inc = 2;
          _hue = 128;
          _fade = 8;
          _hueDelta = 64;
          break;
        case 10:
          _targetPalette = ForestColors_p;
          _inc = 1;
          _hue = random16(255);
          _fade = 1;
          _hueDelta = 16;
          break;
        case 15:
          break;
      }
    }

    EVERY_N_MILLIS_I(confettiTimer, FACTORY_CONFETTI_MODE_DELAY) {
      fadeToBlackBy(leds, numLeds, _fade);
      int pos = random16(numLeds);
      leds[pos] =
          ColorFromPalette(_currentPalette, _hue + random16(_hueDelta) / 4, _state.brightness, _currentBlending);
      _hue = _hue + _inc;
      ledController->showLeds();
      confettiTimer.setPeriod(_state.delay);
    }
  });
}

void ConfettiMode::enable() {
  _refresh = true;
}

void ConfettiMode::sampleComplete() {
}
