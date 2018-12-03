#ifndef AUDIO_LIGHT_MODE_H
#define AUDIO_LIGHT_MODE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLed.h>

class AudioLightMode {
  protected:  
    CLEDController *_ledController;   
    CRGB *_leds;
    int _numLeds;
    uint16_t _numBands;
    uint16_t *_bands;

    void (*configChangeCallback)();

  public:
  
   AudioLightMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *bands, uint16_t numBands) {
     this->_ledController = ledController;
     this->_leds = leds;  
     this->_numLeds = numLeds;
     this->_bands = bands;
     this->_numBands = numBands;
   }

   /*
   * Get the code for the mode as a string
   */
  virtual String getId() = 0;

   /*
   * Allow the mode to animate the LEDs, called by the main loop.
   */
   virtual void tick() = 0;

   /*
   * Called when the bands are sampled and new data is available for rendering.
   * 
   * Tick method can be free running.
   */
   virtual void sampleComplete() = 0;

   /**
   * Called when the effect is enabled.
   */
   virtual void enable() = 0;

   // to update from the web server
   virtual void updateConfig(JsonObject& root) = 0;
   virtual void writeConfig(JsonObject& root) = 0;

};

#endif
