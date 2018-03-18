#include <lightstrip/OffMode.h>

void OffMode::enable() {
  turnOff = true;
}

void OffMode::tick() {
  if (turnOff){
    strip->renderRGB(CRGB::Black, 0);
    turnOff = false;
  }
}

uint64_t OffMode::getModeCode() {
  return IR_OFF;
}
