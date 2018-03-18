#ifndef COLOR_MODE_H
#define COLOR_MODE_H

#include <Arduino.h>

#include <lightstrip/Mode.h>
#include <lightstrip/JsonUtil.h>

#define COLOR_BRIGHTNESS_ADJUST 15
#define COLOR_MIN_BRIGHTNESS 45

class ColorMode : public Mode {

  private:
   CRGB color = CRGB::White;
   uint8_t brightness = 255;
   boolean refresh = true;

  public:
    ColorMode(Strip *strip) : Mode(strip){
      // nothing to do here, the super class manages the strip and callback
    };
    void tick();
    void enable();
    void handleIrCode(uint64_t irCode, boolean repeat);
    uint64_t getModeCode();
    void updateConfig(JsonObject& root);
    void writeConfig(JsonObject& root);
};

#endif
