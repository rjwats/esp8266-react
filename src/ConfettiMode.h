#ifndef CONFETTI_MODE_H
#define CONFETTI_MODE_H

#include <Arduino.h>
#include <FastLed.h>

#include <AudioLightMode.h>
#include <JsonUtil.h>

#define CONFETTI_FILE_PATH "/modes/confetti.json"
#define CONFETTI_DEFAULT_MAX_CHANGES 24
#define CONFETTI_DEFAULT_BRIGHTNESS 255
#define CONFETTI_DEFAULT_DELAY 5

class ConfettiMode : public AudioLightMode {

private:
  
  // palettes for blending
  CRGBPalette16 _currentPalette;
  CRGBPalette16 _targetPalette;
  TBlendType _currentBlending = LINEARBLEND; // NOBLEND or LINEARBLEND

  // Define variables used by the sequences.
  uint8_t _fade = 8;  // How quickly does it fade? Lower = slower fade rate.
  int _hue = 50;       // Starting hue.
  uint8_t _inc = 1;   // Incremental value for rotating hues
  int _hueDelta = 255; // Range of random #'s to use for hue

  // number of changes per cycle
  uint8_t _maxChanges = CONFETTI_DEFAULT_MAX_CHANGES; 
  uint8_t _brightness = CONFETTI_DEFAULT_BRIGHTNESS; // Brightness of a sequence. Remember, max_brightnessght is the overall limiter.
  uint8_t _delay = CONFETTI_DEFAULT_DELAY; // We don't need much delay (if any)

  bool _refresh = true;

public:
  ConfettiMode(FS* fs, CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *bands, uint16_t numBands)
      : AudioLightMode(fs, ledController, leds, numLeds, bands, numBands, CONFETTI_FILE_PATH) {};

  String getId();
  void tick();
  void sampleComplete();
  void enable();
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);

};

#endif
