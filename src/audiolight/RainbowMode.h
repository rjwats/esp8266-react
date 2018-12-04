#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <Arduino.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

#define RAINBOW_DEFAULT_HUE_DELTA 3
#define RAINBOW_DEFAULT_BRIGHTNESS 128

// min and max for rolling avg factors
#define RAINBOW_DEFAULT_ROLLING_AVG_FACTOR 0.5
#define RAINBOW_MAX_ROLLING_AVG_FACTOR 0.99
#define RAINBOW_MIN_ROLLING_AVG_FACTOR 0.01

// rotate speed default and min
#define RAINBOW_DEFAULT_ROTATE_SPEED 128
#define RAINBOW_MIN_ROTATE_SPEED 1

// rainbow mode can be fixed brightness (non-audio enabled)
#define RAINBOW_DEFAULT_AUDIO_ENABLED false

class RainbowMode : public AudioLightMode {

  private:  
    uint8_t _brightness = RAINBOW_DEFAULT_BRIGHTNESS;
    uint8_t _rotateSpeed = RAINBOW_DEFAULT_ROTATE_SPEED;
    bool _audioEnabled = RAINBOW_DEFAULT_AUDIO_ENABLED;
    float _rollingAverageFactor = RAINBOW_DEFAULT_ROLLING_AVG_FACTOR;
    uint8_t _hueDelta = RAINBOW_DEFAULT_HUE_DELTA;

    // various state and settings for rainbow mode
    uint16_t _ledsPerBand;
    uint8_t _remainingLeds;

    // delay state for rotation
    unsigned long _lastFrameMicros = 0;

    uint8_t _initialhue = 0;
    uint16_t *_rollingAverages;
    boolean _refresh = true;

  public:
    RainbowMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *bands, uint16_t numBands) 
    : AudioLightMode(ledController, leds, numLeds, bands, numBands) {
      _rollingAverages = (uint16_t *) malloc(sizeof(uint16_t) * numBands);
      _ledsPerBand = _numLeds / _numBands;
      _remainingLeds = _numLeds % _numBands;  
    };
    String getId();
    void tick();
    void sampleComplete();
    void enable();
    void updateConfig(JsonObject& root);
    void writeConfig(JsonObject& root);
};

#endif
