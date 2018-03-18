#include <lightstrip/SmoothMode.h>

void SmoothMode::enable() {
  rotation = 255;
  nextRotation = millis();
}

void SmoothMode::tick() {
  long now = millis();
  if (nextRotation<=now){
    nextRotation = now + map(SPEED_TABLE[rotationSpeed], 0, 255, SMOOTH_MAX_DELAY, SMOOTH_MIN_DELAY);
    rotation ++;
    strip->renderRGB(CHSV(rotation, 255, 255), 255);
  }
}

void SmoothMode::handleIrCode(uint64_t irCode, boolean repeat){
  switch (irCode) {
    case IR_BPLUS: {
      uint8_t newRotationSpeed = rotationSpeed + SMOOTH_SPEED_ADJUST;
      if (newRotationSpeed < rotationSpeed){
        rotationSpeed = 255;
      }else{
        rotationSpeed = newRotationSpeed;
      }
      break;
    }
    case IR_BMINUS: {
        uint8_t newRotationSpeed = rotationSpeed - SMOOTH_SPEED_ADJUST;
        if (newRotationSpeed > rotationSpeed){
          rotationSpeed = 0;
        }else{
          rotationSpeed = newRotationSpeed;
        }
        break;
    }
  }
}

uint64_t SmoothMode::getModeCode() {
  return IR_SMOOTH;
}

void SmoothMode::updateConfig(JsonObject& root) {
  updateByteFromJson(root, &rotationSpeed, "rotation_speed");
}

void SmoothMode::writeConfig(JsonObject& root) {
  writeByteToJson(root, &rotationSpeed, "rotation_speed");
}
