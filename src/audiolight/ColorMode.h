#ifndef COLOR_MODE_H
#define COLOR_MODE_H

#include <Arduino.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

#define COLOR_BRIGHTNESS_ADJUST 15
#define COLOR_MIN_BRIGHTNESS 45

class ColorMode : public AudioLightMode {

  private:
   CRGB color = CRGB::White;
   uint8_t brightness = 255;
   boolean refresh = true;

  public:
    ColorMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *frequencies) 
    : AudioLightMode(ledController, leds, numLeds, frequencies) {};
    String getId();    
    void tick();
    void enable();
    void updateConfig(JsonObject& root);
    void writeConfig(JsonObject& root);
};

#endif
