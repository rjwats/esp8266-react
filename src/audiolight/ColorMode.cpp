#include <audiolight/ColorMode.h>

String ColorMode::getId() {
  return "color";
}

void ColorMode::enable() {
  _refresh = true;
}

void ColorMode::tick() {
  if (_refresh) {
    fill_solid(_leds, _numLeds, _color);
    _ledController->showLeds(_brightness);
    _refresh = false;
  }
}

void ColorMode::updateConfig(JsonObject& root) {
  updateByteFromJson(root, &_brightness, "brightness");
  updateColorFromJson(root, &_color);
  _refresh = true;
}

void ColorMode::writeConfig(JsonObject& root) {
  writeByteToJson(root, &_brightness, "brightness");
  writeColorToJson(root, &_color);
}
