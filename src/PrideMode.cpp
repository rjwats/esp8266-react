#include <PrideMode.h>

PrideMode::PrideMode(AsyncWebServer* server,
                     FS* fs,
                     SecurityManager* securityManager,
                     LedSettingsService* ledSettingsService,
                     FrequencySampler* frequencySampler) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
                       ledSettingsService,
                       frequencySampler,
                       PrideModeSettings::read,
                       PrideModeSettings::update,
                       PRIDE_MODE_ID) {
  addUpdateHandler([&](const String& originId) { enable(); }, false);
};

void PrideMode::enable() {
  _refresh = true;
}

void PrideMode::tick() {
  _ledSettingsService->update([&](CRGB* leds, CLEDController* ledController, const uint16_t numLeds) {
    static uint16_t sPseudotime = 0;
    static uint16_t sLastMillis = 0;
    static uint16_t sHue16 = 0;

    uint8_t sat8 = beatsin88(87, 220, 250);
    uint8_t brightdepth = beatsin88(341, 96, 224);
    uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
    uint8_t msmultiplier = beatsin88(147, 23, 60);

    uint16_t hue16 = sHue16;  // gHue * 256;
    uint16_t hueinc16 = beatsin88(113, 1, 3000);

    uint16_t ms = millis();
    uint16_t deltams = ms - sLastMillis;
    sLastMillis = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16 += deltams * beatsin88(400, 5, 9);
    uint16_t brightnesstheta16 = sPseudotime;

    for (uint16_t i = 0; i < numLeds; i++) {
      hue16 += hueinc16;
      uint8_t hue8 = hue16 / 256;

      brightnesstheta16 += brightnessthetainc16;
      uint16_t b16 = sin16(brightnesstheta16) + 32768;

      uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
      uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
      bri8 += (255 - brightdepth);

      CRGB newcolor = CHSV(hue8, sat8, bri8);

      uint16_t pixelnumber = i;
      pixelnumber = (numLeds - 1) - pixelnumber;

      nblend(leds[pixelnumber], newcolor, 64);
    }
    
    // Show the leds
    ledController->showLeds();
  });
}
