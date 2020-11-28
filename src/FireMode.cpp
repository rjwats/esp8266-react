#include <FireMode.h>

FireMode::FireMode(AsyncWebServer* server,
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
                       std::bind(&FireMode::read, this, std::placeholders::_1, std::placeholders::_2),
                       std::bind(&FireMode::update, this, std::placeholders::_1, std::placeholders::_2),
                       FIRE_MODE_ID){};

void FireMode::enable() {
  _refresh = true;
  updateWithoutPropagation([&](FireModeSettings& settings) {
    selectPalette(settings.palette.id, settings);
    return StateUpdateResult::CHANGED;
  });
}

void FireMode::tick() {
  if (_refresh) {
    _ledSettingsService->update([&](CRGB* leds, const uint16_t numLeds) {
      fill_solid(leds, numLeds, CHSV(255, 0, 0));      // clear leds
      FastLED.show();                                  // render all leds black
      memset(_heatMap, 0, sizeof(uint8_t) * numLeds);  // clear heat map
      _refresh = false;                                // clear refresh flag
    });
  }
  // make firey stuff at ~100FPS
  EVERY_N_MILLIS(10) {
    _ledSettingsService->update([&](CRGB* leds, const uint16_t numLeds) {
      // Step 1.  Cool down every cell a little
      for (int i = 0; i < numLeds; i++) {
        _heatMap[i] = qsub8(_heatMap[i], random8(0, ((_state.cooling * 10) / numLeds) + 2));
      }

      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for (int k = numLeds - 1; k >= 2; k--) {
        _heatMap[k] = (_heatMap[k - 1] + _heatMap[k - 2] + _heatMap[k - 2]) / 3;
      }

      // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
      if (random8() < _state.sparking) {
        int y = random8(7);
        _heatMap[y] = qadd8(_heatMap[y], random8(160, 255));
      }

      // Step 4.  Map from heat cells to LED colors
      for (int j = 0; j < numLeds; j++) {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        byte colorindex = scale8(_heatMap[j], 240);
        CRGB color = ColorFromPalette(_state.palette.colors, colorindex);
        int pixelnumber;
        if (_state.reverse) {
          pixelnumber = (numLeds - 1) - j;
        } else {
          pixelnumber = j;
        }
        leds[pixelnumber] = color;
      }

      // Step 5. Render the update
      FastLED.show();
    });
  }
}
