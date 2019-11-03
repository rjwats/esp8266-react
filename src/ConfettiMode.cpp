#include <ConfettiMode.h>

String ConfettiMode::getId() {
  return "confetti";
}

void ConfettiMode::enable() {
  _refresh = true;
}

void ConfettiMode::sampleComplete() {};

void ConfettiMode::tick() {
  EVERY_N_MILLISECONDS(100) {
    nblendPaletteTowardPalette(_currentPalette, _targetPalette, _maxChanges);   // AWESOME palette blending capability.
  }

  uint8_t secondHand = (millis() / 1000) % 15; // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;              // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {              // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: _targetPalette = OceanColors_p; _inc=1; _hue=192; _fade=2; _hueDelta=255; break;  // You can change values here, one at a time , or altogether.
      case  5: _targetPalette = LavaColors_p; _inc=2; _hue=128; _fade=8; _hueDelta=64; break;
      case 10: _targetPalette = ForestColors_p; _inc=1; _hue=random16(255); _fade=1; _hueDelta=16; break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;                                         // Here's the matching 15 for the other one.
    }
  }

  EVERY_N_MILLIS_I(confettiTimer, CONFETTI_DEFAULT_DELAY) {
    fadeToBlackBy(_leds, _numLeds, _fade);
    int pos = random16(_numLeds);
    _leds[pos] = ColorFromPalette(_currentPalette, _hue + random16(_hueDelta)/4 , _brightness, _currentBlending);
    _hue = _hue + _inc;
    _ledController->showLeds();    
    confettiTimer.setPeriod(_delay);
  }

}

void ConfettiMode::readFromJsonObject(JsonObject& root) {
  updateByteFromJson(root, &_maxChanges, CONFETTI_DEFAULT_MAX_CHANGES, "max_changes");
  updateByteFromJson(root, &_brightness, CONFETTI_DEFAULT_BRIGHTNESS, "brightness");
  updateByteFromJson(root, &_delay, CONFETTI_DEFAULT_DELAY, "delay");
  _refresh = true;
}

void ConfettiMode::writeToJsonObject(JsonObject& root) {
  writeByteToJson(root, &_maxChanges, "max_changes");
  writeByteToJson(root, &_brightness, "brightness");
  writeByteToJson(root, &_delay, "delay");
}
