#ifndef OFF_MODE_H
#define OFF_MODE_H

#include <Arduino.h>

#include <audiolight/AudioLightMode.h>

class OffMode : public AudioLightMode {

  private:
   boolean _turnOff;

  public:
    OffMode(CLEDController *ledController, CRGB *leds, int numLeds, uint16_t *frequencies) 
    : AudioLightMode(ledController, leds, numLeds, frequencies){};
    String getId();    
    void tick();
    void enable();
    void updateConfig(JsonObject& root) {};
    void writeConfig(JsonObject& root) {};    
};

#endif
