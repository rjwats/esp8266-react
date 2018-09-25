#include <audiolight/OffMode.h>

String OffMode::getId() {
  return "off";
}

void OffMode::enable() {
  turnOff = true;
}

void OffMode::tick() {
  // turn off by fading?
}


