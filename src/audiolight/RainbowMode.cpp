#include <audiolight/RainbowMode.h>

String RainbowMode::getId() {
  return "rainbow";
}

void RainbowMode::enable() {
  _refresh = true;
}

void RainbowMode::tick() {
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
