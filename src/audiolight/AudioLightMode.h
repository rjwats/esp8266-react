#ifndef AUDIO_LIGHT_MODE_H
#define AUDIO_LIGHT_MODE_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "FastLED.h"

class AudioLightMode {
  protected:  
    CLEDController *_ledController;   
    CRGB *_leds;
    int _numLeds;
    uint16_t *frequencies;

    void (*configChangeCallback)();

  public:

   AudioLightMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *frequencies) {
     this->_ledController = ledController;
     this->_leds = leds;  
     this->_numLeds = numLeds;
     this->frequencies = frequencies;
   }

   /*
   * Get the code for the mode as a string
   */
  virtual String getMode() = 0;

   /*
   * Allow the mode to animate the LEDs, called by the main loop.
   */
   virtual void tick() = 0;

   /**
   * Called when the effect is enabled.
   */
   virtual void enable() = 0;

   // to update from the web server
   virtual void updateConfig(JsonObject& root) = 0;
   virtual void writeConfig(JsonObject& root) = 0;

};

#endif
