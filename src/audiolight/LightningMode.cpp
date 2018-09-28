#include <audiolight/LightningMode.h>

String LightningMode::getId() {
  return "lightning";
}

void LightningMode::enable() {
  _refresh = true;
}

void LightningMode::tick() {
 
}

void LightningMode::updateConfig(JsonObject &root) {
  updateByteFromJson(root, &_brightness, "brightness");
  _refresh = true;
}

void LightningMode::writeConfig(JsonObject &root) {
  writeByteToJson(root, &_brightness, "brightness");
}
