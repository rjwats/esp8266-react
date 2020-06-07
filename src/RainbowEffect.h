#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <Arduino.h>

#include <LightEffect.h>
#include <JsonUtil.h>
#include <StatefulService.h>

#define RAINBOW_DEFAULT_ROTATE_SPEED 32
#define RAINBOW_DEFAULT_HUE_DELTA 250

class RainbowEffectSettings {
 public:
  uint8_t rotateSpeed;
  uint8_t hueDelta;
};

class RainbowEffect : public LightEffectService<RainbowEffectSettings> {
 private:
  unsigned long _lastFrameMicros = 0;
  uint8_t _initialhue = 0;
  bool _refresh = true;

 public:
  RainbowEffect(CLEDController* ledController) : LightEffectService(ledController){};

  String getId() {
    return "Rainbow";
  }

  void activate() {
    _lastFrameMicros = micros();
  }

  void loop() {
    if (_state.rotateSpeed > 0) {
      unsigned long rotateDelayMicros = 1000000 / _state.rotateSpeed;
      unsigned long currentMicros = micros();
      unsigned long microsElapsed = (unsigned long)(currentMicros - _lastFrameMicros);
      if (microsElapsed >= rotateDelayMicros) {
        _lastFrameMicros = currentMicros;
        _initialhue++;
      }
    }
    fill_rainbow(
        LightEffect::_ledController->leds(), LightEffect::_ledController->size(), _initialhue, _state.hueDelta);
    FastLED.show();
  }

  void readSettings(RainbowEffectSettings& settings, JsonObject& root) {
    root["rotate_speed"] = settings.rotateSpeed;
    root["hue_delta"] = settings.hueDelta;
  }

  StateUpdateResult updateSettings(JsonObject& root, RainbowEffectSettings& settings) {
    settings.rotateSpeed = root["rotate_speed"] | RAINBOW_DEFAULT_ROTATE_SPEED;
    settings.hueDelta = root["hue_delta"] | RAINBOW_DEFAULT_HUE_DELTA;
    return StateUpdateResult::CHANGED;
  }
};

#endif
