#ifndef CONFETTI_MODE_H
#define CONFETTI_MODE_H

#include <Arduino.h>
#include <FastLed.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

#define CONFETTI_FILE_PATH "/modes/confetti.json"

class ConfettiMode : public AudioLightMode {

private:
  
  // palettes for blending
  CRGBPalette16 _currentPalette;
  CRGBPalette16 _targetPalette;
  TBlendType    _currentBlending = LINEARBLEND; // NOBLEND or LINEARBLEND
  // number of changes per cycle
  uint8_t _maxChanges = 24; 

  // Define variables used by the sequences.
  uint8_t  _thisfade = 8;  // How quickly does it fade? Lower = slower fade rate.
  int      _thishue = 50;  // Starting hue.
  uint8_t  _thisinc = 1;   // Incremental value for rotating hues
  uint8_t  _thissat = 100; // The saturation, where 255 = brilliant colours.
  uint8_t  _thisbri = 255; // Brightness of a sequence. Remember, max_bright is the overall limiter.
  int      _huediff = 256; // Range of random #'s to use for hue
  uint8_t  _thisdelay = 5; // We don't need much delay (if any)

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
