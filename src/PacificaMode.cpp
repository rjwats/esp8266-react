#include <PacificaMode.h>

PacificaMode::PacificaMode(AsyncWebServer* server,
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
                       std::bind(&PacificaMode::read, this, std::placeholders::_1, std::placeholders::_2),
                       std::bind(&PacificaMode::update, this, std::placeholders::_1, std::placeholders::_2),
                       PACIFICA_MODE_ID) {
  addUpdateHandler([&](const String& originId) { enable(); }, false);
};

void PacificaMode::enable() {
  _refresh = true;
  updateWithoutPropagation([&](PacificaModeSettings& settings) {
    _paletteSettingsService->refreshPalette(settings.palette1);
    _paletteSettingsService->refreshPalette(settings.palette2);
    _paletteSettingsService->refreshPalette(settings.palette3);
    return StateUpdateResult::CHANGED;
  });
}

void PacificaMode::tick() {
  _ledSettingsService->update([&](CRGB* leds, CLEDController* ledController, const uint16_t numLeds) {
    // Increment the four "color index start" counters, one for each wave layer.
    // Each is incremented at a different speed, and the speeds vary over time.
    static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
    static uint32_t sLastms = 0;
    uint32_t ms = millis();
    uint32_t deltams = ms - sLastms;
    sLastms = ms;
    uint16_t speedfactor1 = beatsin16(3, 179, 269);
    uint16_t speedfactor2 = beatsin16(4, 179, 269);
    uint32_t deltams1 = (deltams * speedfactor1) / 256;
    uint32_t deltams2 = (deltams * speedfactor2) / 256;
    uint32_t deltams21 = (deltams1 + deltams2) / 2;
    sCIStart1 += (deltams1 * beatsin88(1011, 10, 13));
    sCIStart2 -= (deltams21 * beatsin88(777, 8, 11));
    sCIStart3 -= (deltams1 * beatsin88(501, 5, 7));
    sCIStart4 -= (deltams2 * beatsin88(257, 4, 6));

    // Clear out the LED array to a dim background blue-green
    fill_solid(leds, numLeds, CRGB(2, 6, 10));

    // Render each of four layers, with different scales and speeds, that vary over time
    pacifica_one_layer(leds,
                       numLeds,
                       _state.palette1.colors,
                       sCIStart1,
                       beatsin16(3, 11 * 256, 14 * 256),
                       beatsin8(10, 70, 130),
                       0 - beat16(301));
    pacifica_one_layer(leds,
                       numLeds,
                       _state.palette2.colors,
                       sCIStart2,
                       beatsin16(4, 6 * 256, 9 * 256),
                       beatsin8(17, 40, 80),
                       beat16(401));
    pacifica_one_layer(leds, numLeds, _state.palette3.colors, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
    pacifica_one_layer(leds, numLeds, _state.palette3.colors, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

    // Add brighter 'whitecaps' where the waves lines up more
    pacifica_add_whitecaps(leds, numLeds);

    // Deepen the blues and greens a bit
    pacifica_deepen_colors(leds, numLeds);

    // Show the leds
    ledController->showLeds();
  });
}

// Add one layer of waves into the led array
void PacificaMode::pacifica_one_layer(CRGB* leds,
                                      const uint16_t numLeds,
                                      CRGBPalette16& p,
                                      uint16_t cistart,
                                      uint16_t wavescale,
                                      uint8_t bri,
                                      uint16_t ioff) {
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for (uint16_t i = 0; i < numLeds; i++) {
    waveangle += 250;
    uint16_t s16 = sin16(waveangle) + 32768;
    uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16(ci) + 32768;
    uint8_t sindex8 = scale16(sindex16, 240);
    CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void PacificaMode::pacifica_add_whitecaps(CRGB* leds, const uint16_t numLeds) {
  uint8_t basethreshold = beatsin8(9, 55, 65);
  uint8_t wave = beat8(7);

  for (uint16_t i = 0; i < numLeds; i++) {
    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      leds[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void PacificaMode::pacifica_deepen_colors(CRGB* leds, const uint16_t numLeds) {
  for (uint16_t i = 0; i < numLeds; i++) {
    leds[i].blue = scale8(leds[i].blue, 145);
    leds[i].green = scale8(leds[i].green, 200);
    leds[i] |= CRGB(2, 5, 7);
  }
}
