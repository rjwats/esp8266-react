#include <audiolight/RainbowMode.h>

String RainbowMode::getId() {
  return "rainbow";
}

void RainbowMode::enable() {
  _refresh = true;
}

void RainbowMode::tick() {
EVERY_N_MILLIS(100){
  // TODO - calculate once
  uint16_t ledsPerBand = _numLeds / _numBands;
  uint8_t remainingLeds = _numLeds % _numBands;  

  /* Approach 1 uses each frequency in turn*/
  uint16_t currentLed = 0;
  uint8_t hue = _initialhue++;
  for (uint8_t i = 0; i<_numBands; i++) {
    // handle rolling averages
    _rollingAverages[i] = _rollingAverageFactor * _bands[i] + (1 - _rollingAverageFactor) * _rollingAverages[i];
        
    // paint rainbow and fade according to level of each frequency
    fill_rainbow(_leds + currentLed, ledsPerBand + (i ==_numLeds -1 ? remainingLeds: 0), hue, _hueDelta);
    fadeToBlackBy(_leds + currentLed, ledsPerBand + (i ==_numLeds -1 ? remainingLeds: 0), 255-map(_rollingAverages[i], 0, 4096, 0, 255));

    // increment hue and current led
    hue += _hueDelta * ledsPerBand;
    currentLed += ledsPerBand;

    _ledController->showLeds(255);
  }}
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
