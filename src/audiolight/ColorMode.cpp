#include <audiolight/ColorMode.h>

String ColorMode::getId() {
  return "color";
}

void ColorMode::enable() {
  refresh = true;
}

void ColorMode::tick() {
  if (refresh) {
    fill_solid( _leds, _numLeds, color);
    
    refresh = false;
  }
}

void ColorMode::updateConfig(JsonObject& root) {
  updateByteFromJson(root, &brightness, "brightness");
  updateColorFromJson(root, &color);
  refresh = true;
}

void ColorMode::writeConfig(JsonObject& root) {
  writeByteToJson(root, &brightness, "brightness");
  writeColorToJson(root, &color);
}
