#ifndef SPECTRUM_MODE_H
#define SPECTRUM_MODE_H

#include <Arduino.h>
#include <FastLed.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

#define MIN_PEAK_DECAY_AMOUNT 1
#define MAX_SAMPLE_SIZE 1024

class SpectrumMode : public AudioLightMode {

private:
  // not used.. to be replaced
  uint8_t _brightness = 255;
  boolean _refresh = true;

  // How long should it take the peaks to decay from 100% to 0%
  uint16_t _peakDecayMs = 1000;
  uint16_t _peakDecayAmount = 0;
  uint16_t _peaks[7];

  // Have a go generating rolling averages
  float _rollingAverageFactor = 0.7;
  uint16_t _rollingAverages[7];

public:
  SpectrumMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *frequencies)
      : AudioLightMode(ledController, leds, numLeds, frequencies){};
  String getId();
  void tick();
  void enable();
  void updateConfig(JsonObject &root);
  void writeConfig(JsonObject &root);
};

#endif
