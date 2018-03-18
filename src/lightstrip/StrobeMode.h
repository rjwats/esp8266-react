#ifndef STROBE_MODE_H
#define STROBE_MODE_H

#include <lightstrip/Mode.h>
#include <lightstrip/SpeedTable.h>
#include <lightstrip/JsonUtil.h>

#define STROBE_SPEED_ADJUST 15
#define STROBE_MAX_DURATION 1000
#define STROBE_MIN_DURATION 50

class StrobeMode : public Mode {

  private:
   CRGB color = CRGB::White;
   uint8_t strobeSpeed = 100;
   uint8_t strobeState = 0;
   uint64_t nextStrobe = 0;

  public:
    StrobeMode(Strip *strip) : Mode(strip){
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
