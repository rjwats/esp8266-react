#ifndef AUDIO_LIGHT_MODE_H
#define AUDIO_LIGHT_MODE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLed.h>
#include <audiolight/AudioLight.h>

class AudioLightMode {
  protected:  
    CLEDController *_ledController;   
    CRGB *_leds;
    int _numLeds;
    uint16_t _numBands;
    uint16_t *_bands;

    void (*configChangeCallback)();

    /**
     * Calculates the energy level as a total of the maximum possible value in the range 0 to 1.
     */
    float calculateEnergyFloat(bool *includedBands=NULL) {
      uint16_t currentLevel = 0;
      uint16_t numBands = 0;
      for (uint16_t i=0; i<_numBands; i++) {
        if (includedBands == NULL || includedBands[i]) {
          currentLevel += _bands[i];
          numBands++;
        }
      }
      return (float) currentLevel / (_numBands * ADC_MAX_VALUE);
    }

    uint8_t calculateEnergyPercentage(bool *includedBands=NULL) {
      return calculateEnergyFloat(includedBands) * 100;
    }

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
