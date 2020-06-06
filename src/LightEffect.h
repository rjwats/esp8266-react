#ifndef LightEffect_h
#define LightEffect_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLed.h>
#include <StatefulService.h>

class LightEffect {
 protected:
  CLEDController* _ledController;

 public:
  LightEffect(CLEDController* ledController) : _ledController(ledController) {
  }

  /*
   * Allow the effect to control the LEDs.
   *
   * Called by the main loop on the active effect.
   */
  virtual void loop() = 0;

  /**
   * Called when the effect is activated.
   */
  virtual void activate() = 0;
};

#endif
