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

  public:
    RainbowMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *frequencies) 
    : AudioLightMode(ledController, leds, numLeds, frequencies) {};
    String getId();    
    void tick();
    void enable();
    void updateConfig(JsonObject& root);
    void writeConfig(JsonObject& root);
};

#endif
