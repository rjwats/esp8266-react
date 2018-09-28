#ifndef SPECTRUM_MODE_H
#define SPECTRUM_MODE_H

#include <Arduino.h>
#include <FastLed.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

class LightningMode : public AudioLightMode
{

private:
  uint8_t _brightness = 255 boolean _refresh = true;

  uint8_t _frequency = 50; // controls the interval between strikes
  uint8_t _flashes = 8;    //the upper limit of flashes per strike
  uint16_t _dimmer = 1;

  uint8_t _ledstart; // Starting location of a flash
  uint8_t _ledlen;   // Length of a flash

public:
  LightningMode(CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *frequencies)
      : AudioLightMode(ledController, leds, numLeds, frequencies){};
  String getId();
  void tick();
  void enable();
  void updateConfig(JsonObject &root);
  void writeConfig(JsonObject &root);
};

#endif
