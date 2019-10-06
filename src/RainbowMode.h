#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <Arduino.h>

#include <AudioLightMode.h>
#include <JsonUtil.h>

#define RAINBOW_DEFAULT_HUE_DELTA 250
#define RAINBOW_DEFAULT_BRIGHTNESS 128

// rotate speed default and min
#define RAINBOW_DEFAULT_ROTATE_SPEED 32

// rainbow mode can be fixed brightness (non-audio enabled)
#define RAINBOW_DEFAULT_AUDIO_ENABLED false

#define RAINBOW_FILE_PATH "/modes/rainbow.json"

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
    RainbowMode(FS* fs, CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *bands, uint16_t numBands) 
    : AudioLightMode(fs, ledController, leds, numLeds, bands, numBands, RAINBOW_FILE_PATH) {
      _ledsPerBand = _numLeds / _numBands;
      _remainingLeds = _numLeds % _numBands;  
    };
    String getId();
    void tick();
    void sampleComplete() {};
    void enable();
    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);
};

#endif
