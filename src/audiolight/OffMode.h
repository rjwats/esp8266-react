#ifndef OFF_MODE_H
#define OFF_MODE_H

#define LIGHTNING_FILE_PATH "/modes/lightning.json"

#include <Arduino.h>
#include <audiolight/AudioLightMode.h>

class OffMode : public AudioLightMode {

  private:
   boolean _turnOff;

  public:
    OffMode(FS* fs, CLEDController *ledController, CRGB *leds, int numLeds, uint16_t *bands, uint16_t numBands) 
    : AudioLightMode(fs, ledController, leds, numLeds, bands, numBands, LIGHTNING_FILE_PATH){};
    String getId();    
    void tick();
    void sampleComplete(){};
    void enable();
    void readFromJsonObject(JsonObject& root) {};
    void writeToJsonObject(JsonObject& root) {};
};

#endif
