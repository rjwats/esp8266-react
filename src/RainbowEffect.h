#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <Arduino.h>

#include <LightEffect.h>
#include <JsonUtil.h>
#include <StatefulService.h>

#define RAINBOW_DEFAULT_HUE_DELTA 250
#define RAINBOW_DEFAULT_BRIGHTNESS 128

// rotate speed default and min
#define RAINBOW_DEFAULT_ROTATE_SPEED 32

// rainbow mode can be fixed brightness (non-audio enabled)
#define RAINBOW_DEFAULT_AUDIO_ENABLED false

#define RAINBOW_FILE_PATH "/modes/rainbow.json"

class RainbowEffectSettings {
 public:
  uint8_t rotateSpeed = RAINBOW_DEFAULT_ROTATE_SPEED;
  uint8_t hueDelta = RAINBOW_DEFAULT_HUE_DELTA;
};

class RainbowEffect : public LightEffect, public StatefulService<RainbowEffectSettings> {
 private:
  unsigned long _lastFrameMicros = 0;
  uint8_t _initialhue = 0;
  bool _refresh = true;

 public:
  RainbowEffect(CLEDController* ledController) : LightEffect(ledController){};
  void loop();
  void activate();
};

#endif
