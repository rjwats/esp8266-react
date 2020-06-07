#ifndef CONFETTI_MODE_H
#define CONFETTI_MODE_H

#include <Arduino.h>
#include <FastLed.h>
#include <LightEffect.h>
#include <JsonUtil.h>

#define CONFETTI_DEFAULT_MAX_CHANGES 24
#define CONFETTI_DEFAULT_DELAY 5

class ConfettiEffectSettings {
 public:
  uint8_t maxChanges = CONFETTI_DEFAULT_MAX_CHANGES;
  uint8_t delay = CONFETTI_DEFAULT_DELAY;
};

class ConfettiEffect : public LightEffectService<ConfettiEffectSettings> {
 private:
  // palettes for blending
  CRGBPalette16 _currentPalette;
  CRGBPalette16 _targetPalette;
  TBlendType _currentBlending = LINEARBLEND;  // NOBLEND or LINEARBLEND

  // Define variables used by the sequences.
  uint8_t _fade = 8;    // How quickly does it fade? Lower = slower fade rate.
  int _hue = 50;        // Starting hue.
  uint8_t _inc = 1;     // Incremental value for rotating hues
  int _hueDelta = 255;  // Range of random #'s to use for hue

  bool _refresh = true;

 public:
  ConfettiEffect(CLEDController* ledController) : LightEffectService(ledController){};

  String getId() {
    return "Confetti";
  }

  void loop() {
    EVERY_N_MILLISECONDS(100) {
      nblendPaletteTowardPalette(
          _currentPalette, _targetPalette, _state.maxChanges);  // AWESOME palette blending capability.
    }

    uint8_t secondHand = (millis() / 1000) % 15;  // IMPORTANT!!! Change '15' to a different
    static uint8_t lastSecond = 99;  // Static variable, means it's only defined once. This is our 'debounce' variable.
    if (lastSecond != secondHand) {  // Debounce to make sure we're not repeating an assignment.
      lastSecond = secondHand;
      switch (secondHand) {
        case 0:
          _targetPalette = OceanColors_p;
          _inc = 1;
          _hue = 192;
          _fade = 2;
          _hueDelta = 255;
          break;  // You can change values here, one at a time , or altogether.
        case 5:
          _targetPalette = LavaColors_p;
          _inc = 2;
          _hue = 128;
          _fade = 8;
          _hueDelta = 64;
          break;
        case 10:
          _targetPalette = ForestColors_p;
          _inc = 1;
          _hue = random16(255);
          _fade = 1;
          _hueDelta = 16;
          break;  // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
        case 15:
          break;  // Here's the matching 15 for the other one.
      }
    }

    EVERY_N_MILLIS_I(confettiTimer, CONFETTI_DEFAULT_DELAY) {
      fadeToBlackBy(_ledController->leds(), _ledController->size(), _fade);
      int pos = random16(_ledController->size());
      _ledController->leds()[pos] =
          ColorFromPalette(_currentPalette, _hue + random16(_hueDelta) / 4, 255, _currentBlending);
      _hue = _hue + _inc;
      confettiTimer.setPeriod(_state.delay);
      FastLED.show();
    }
  }

  void activate() {
    _refresh = true;
  }

  void readSettings(ConfettiEffectSettings& settings, JsonObject& root) {
    root["max_changes"] = settings.maxChanges;
    root["delay"] = settings.delay;
  }

  StateUpdateResult updateSettings(JsonObject& root, ConfettiEffectSettings& settings) {
    settings.maxChanges = root["max_changes"] | CONFETTI_DEFAULT_MAX_CHANGES;
    settings.delay = root["delay"] | CONFETTI_DEFAULT_DELAY;
    return StateUpdateResult::CHANGED;
  }
};

#endif
