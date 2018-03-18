#ifndef OFF_MODE_H
#define OFF_MODE_H

#include <Arduino.h>

#include <lightstrip/Mode.h>

class OffMode : public Mode {

  private:
   boolean turnOff;

  public:
    OffMode(Strip *strip) : Mode(strip){
      // nothing to do here, the super class manages the strip and callback
    };
    void tick();
    void enable();
    uint64_t getModeCode();
    void updateConfig(JsonObject& root) {};
    void writeConfig(JsonObject& root) {};    
};

#endif
