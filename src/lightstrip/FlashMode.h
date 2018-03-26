#ifndef TWO_COLOR_H
#define TWO_COLOR_H

#include <Arduino.h>

#include <lightstrip/SpeedTable.h>
#include <lightstrip/Mode.h>
#include <lightstrip/JsonUtil.h>

#define FADE_SPEED_ADJUST 15
#define FADE_MAX_DELAY 100
#define FADE_MIN_DELAY 2

class FlashMode : public Mode {

  private:
   CRGB colorOne = CRGB::Red;
   CRGB colorTwo = CRGB::Blue;
   uint8_t fadeSpeed = 100;
   uint8_t fade = 0;
   uint64_t nextFade = 0;
   CRGB* fadeFrom = &colorOne;
   CRGB* fadeTo = &colorTwo;

   CRGB calculateFadeColor();
   void nblendU8TowardU8(uint8_t& cur, const uint8_t target, uint8_t amount);

  public:
    FlashMode(Strip *strip) : Mode(strip){
      // nothing to do here, the super class manages the strip
    };
    void tick();
    void enable();
    void handleIrCode(uint64_t irCode, boolean repeat);
    uint64_t getModeCode();
    void updateConfig(JsonObject& root);
    void writeConfig(JsonObject& root);

};

#endif
