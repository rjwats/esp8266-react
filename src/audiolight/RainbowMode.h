#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <Arduino.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

#define COLOR_BRIGHTNESS_ADJUST 15
#define COLOR_MIN_BRIGHTNESS 45

class RainbowMode : public AudioLightMode {

  private:
    // not used.. to be replaced
    uint8_t _brightness = 255;
    CRGB _color = CRGB::White;

    // various state and settings for rainbow mode
    float _smoothingFactor = 0.7;
    uint8_t _initialhue = 0;
    uint8_t _hueDelta = 3;
    boolean _refresh = true;
    uint16_t _totalEnergy = 0;

  // smoothed readings - using rolling average
  float _rollingAverageFactor = 0.5;
  uint16_t *_rollingAverages;

  public:
    RainbowMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds,  uint16_t *bands, uint16_t numBands) 
    : AudioLightMode(ledController, leds, numLeds, bands, numBands) {
      _rollingAverages = (uint16_t *) malloc(sizeof(uint16_t) * numBands);
    };
    String getId();
    void tick();
    void sampleComplete() {};
    void enable();
    void updateConfig(JsonObject& root);
    void writeConfig(JsonObject& root);
};

#endif
