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
      case  0: _targetPalette = OceanColors_p; _thisinc=1; _thishue=192; _thissat=255; _thisfade=2; _huediff=255; break;  // You can change values here, one at a time , or altogether.
      case  5: _targetPalette = LavaColors_p; _thisinc=2; _thishue=128; _thisfade=8; _huediff=64; break;
      case 10: _targetPalette = ForestColors_p; _thisinc=1; _thishue=random16(255); _thisfade=1; _huediff=16; break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;                                         // Here's the matching 15 for the other one.
    }
  }

  // draw at 100 FPS
  EVERY_N_MILLISECONDS(10) {
    fadeToBlackBy(_leds, _numLeds, _thisfade);
    int pos = random16(_numLeds);
    _leds[pos] = ColorFromPalette(_currentPalette, _thishue + random16(_huediff)/4 , _thisbri, _currentBlending);
    _thishue = _thishue + _thisinc;
    _ledController->showLeds();
  }
}

void ConfettiMode::readFromJsonObject(JsonObject &root) {
  _refresh = true;
}

void ConfettiMode::writeToJsonObject(JsonObject &root) {
}
