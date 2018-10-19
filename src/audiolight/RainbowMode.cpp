#include <audiolight/RainbowMode.h>

String RainbowMode::getId() {
  return "rainbow";
}

void RainbowMode::enable() {
  _refresh = true;
}

void RainbowMode::tick() {
  // TODO - calculate once
  uint16_t ledsPerBand = _numLeds / 7;
  uint8_t remainingLeds = _numLeds % 7;
  

  /* Approach 1 uses each frequency in turn*/
  // TODO - Averages to smooth flickering?
  uint16_t currentLed = 0;
  uint8_t hue = _initialhue++;
  for (uint8_t i = 0; i<7; i++) {
    // handle rolling averages
    _rollingAverages[i] = _rollingAverageFactor * _frequencies[i] + (1 - _rollingAverageFactor) * _rollingAverages[i];
        
    // paint rainbow and fade according to level of each frequency
    fill_rainbow(_leds + currentLed, ledsPerBand + (i ==6 ? remainingLeds: 0), hue, _hueDelta);
    fadeToBlackBy(_leds + currentLed, ledsPerBand + (i ==6 ? remainingLeds: 0), 255-map(_rollingAverages[i], 0, 1024, 0, 255));

    // increment hue and current led
    hue += _hueDelta * ledsPerBand;
    currentLed += ledsPerBand;

    _ledController->showLeds(255);
  }

  /* Approach 2 relys on _totalEnergy
  uint16_t currentEnergy = 0;
  currentEnergy += _frequencies[0];
  currentEnergy += _frequencies[1];
  currentEnergy += _frequencies[2];
  currentEnergy += _frequencies[3];
  currentEnergy += _frequencies[4];
  currentEnergy += _frequencies[5];
  currentEnergy += _frequencies[6]; 

  // calculate running average
  _totalEnergy = _smoothingFactor * currentEnergy + (1 - _smoothingFactor) * _totalEnergy;

  // re-draw baby
  uint8_t brightness = map(_totalEnergy, 0, 7168, 0, 255);
  fill_rainbow(_leds, _numLeds, _initialhue++, _hueDelta);
  _ledController->showLeds(brightness); 
  */
}

void RainbowMode::updateConfig(JsonObject& root) {
  updateByteFromJson(root, &_brightness, "brightness");
  updateColorFromJson(root, &_color);
  _refresh = true;
}

void RainbowMode::writeConfig(JsonObject& root) {
  writeByteToJson(root, &_brightness, "brightness");
  writeColorToJson(root, &_color);
}
