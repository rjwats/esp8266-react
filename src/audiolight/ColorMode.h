#ifndef COLOR_MODE_H
#define COLOR_MODE_H

#include <Arduino.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

#define COLOR_BRIGHTNESS_ADJUST 15
#define COLOR_MIN_BRIGHTNESS 45

class ColorMode : public AudioLightMode {

  private:
   CRGB _color = CRGB::White;
   uint8_t _brightness = 128;
   boolean _refresh = true;

  public:
    ColorMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *bands, uint16_t numBands) 
    : AudioLightMode(ledController, leds, numLeds, bands, numBands) {};
    String getId();
    void tick();
    void sampleComplete() {};
    void enable();
    void updateConfig(JsonObject& root);
    void writeConfig(JsonObject& root);
};

#endif
