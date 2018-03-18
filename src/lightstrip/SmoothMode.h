#ifndef SMOOTH_MODE_H
#define SMOOTH_MODE_H

#include <Arduino.h>

#include <lightstrip/Mode.h>
#include <lightstrip/SpeedTable.h>
#include <lightstrip/JsonUtil.h>

#define SMOOTH_SPEED_ADJUST 15
#define SMOOTH_MAX_DELAY 150
#define SMOOTH_MIN_DELAY 3

class SmoothMode : public Mode {

  private:
   uint8_t rotationSpeed = 100;
   uint8_t rotation = 255;
   uint64_t nextRotation = 0;

  public:
    SmoothMode(Strip *strip) : Mode(strip){
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
