#include <ColorMode.h>

String ColorMode::getId() {
  return "color";
}

void ColorMode::enable() {
  _refresh = true;
}

void ColorMode::tick() {
  if (_refresh || _audioEnabled) {
    fill_solid(_leds, _numLeds, _color);
    _ledController->showLeds(_audioEnabled ? calculateEnergyFloat(_includedBands) * _brightness : _brightness);
    _refresh = false;
  }
}
