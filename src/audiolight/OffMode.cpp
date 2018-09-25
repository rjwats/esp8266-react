#include <audiolight/OffMode.h>

String OffMode::getId() {
  return "off";
}

void OffMode::enable() {
  _turnOff = true;
}

void OffMode::tick() {
  if (_turnOff) {
    _ledController->clearLeds(_numLeds);
    _turnOff = false;
  }
}


