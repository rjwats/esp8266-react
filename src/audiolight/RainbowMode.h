#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <Arduino.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

#define RAINBOW_DEFAULT_HUE_DELTA 64
#define RAINBOW_DEFAULT_BRIGHTNESS 128

// rotate speed default and min
#define RAINBOW_DEFAULT_ROTATE_SPEED 32

// rainbow mode can be fixed brightness (non-audio enabled)
#define RAINBOW_DEFAULT_AUDIO_ENABLED false

class RainbowMode : public AudioLightMode {

  private:  
    uint8_t _brightness = RAINBOW_DEFAULT_BRIGHTNESS;
    uint8_t _rotateSpeed = RAINBOW_DEFAULT_ROTATE_SPEED;
    bool _audioEnabled = RAINBOW_DEFAULT_AUDIO_ENABLED;
    uint8_t _hueDelta = RAINBOW_DEFAULT_HUE_DELTA;

    // various state and settings for rainbow mode
    uint16_t _ledsPerBand;
    uint8_t _remainingLeds;

    // delay state for rotation
    unsigned long _lastFrameMicros = 0;

    uint8_t _initialhue = 0;
    boolean _refresh = true;

  public:
    RainbowMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *bands, uint16_t numBands) 
    : AudioLightMode(ledController, leds, numLeds, bands, numBands) {
      _ledsPerBand = _numLeds / _numBands;
      _remainingLeds = _numLeds % _numBands;  
    };
    String getId();
    void tick();
    void sampleComplete() {};
    void enable();
    void updateConfig(JsonObject& root);
    void writeConfig(JsonObject& root);
};

#endif
