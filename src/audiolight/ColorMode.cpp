#include <audiolight/ColorMode.h>

String ColorMode::getId() {
  return "color";
}

void ColorMode::enable() {
  _refresh = true;
}

void ColorMode::tick() {
  if (_refresh || _audioEnabled) {
    fill_solid(_leds, _numLeds, _color);
    _ledController->showLeds(_audioEnabled ? calculateEnergyFloat() * _brightness : _brightness);
    _refresh = false;
  }
}

void ColorMode::updateConfig(JsonObject& root) {
  updateColorFromJson(root, &_color, COLOR_DEFAULT_COLOR);
  updateBoolFromJson(root, &_audioEnabled, COLOR_DEFAULT_AUDIO_ENABLED, "audio_enabled");
  updateByteFromJson(root, &_brightness, COLOR_DEFAULT_BRIGHTNESS, "brightness");
  _refresh = true;
}

void ColorMode::writeConfig(JsonObject& root) {
  writeColorToJson(root, &_color); 
  writeBoolToJson(root, &_audioEnabled, "audio_enabled");
  writeByteToJson(root, &_brightness, "brightness");
}
