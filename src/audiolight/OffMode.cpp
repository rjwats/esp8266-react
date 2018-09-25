#include <audiolight/OffMode.h>

String OffMode::getMode() {
  return "off";
}

void OffMode::enable() {
  turnOff = true;
}

void OffMode::tick() {
  // turn off by fading?
}


