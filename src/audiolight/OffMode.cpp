#include <audiolight/OffMode.h>

String ColorMode::getMode() {
  return "off";
}

void OffMode::enable() {
  turnOff = true;
}

void OffMode::tick() {
  _ledController.fadeToBlackBy(64);
}


