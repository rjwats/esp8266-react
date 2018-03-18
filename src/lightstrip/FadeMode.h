#ifndef FADE_MODE_H
#define FADE_MODE_H

#include <Arduino.h>

#include <lightstrip/SpeedTable.h>
#include <lightstrip/Mode.h>
#include <lightstrip/JsonUtil.h>

#define FADE_SPEED_ADJUST 15
#define FADE_MAX_DELAY 150
#define FADE_MIN_DELAY 3
#define FADE_BY_MAX 210

class FadeMode : public Mode {

  private:
   CRGB color = CRGB::White;
   uint8_t fadeSpeed = 100;
   uint8_t fade = 0;
   uint8_t fadeDirection = 0;
   uint64_t nextFade = 0;

  public:
    FadeMode(Strip *strip) : Mode(strip){
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
