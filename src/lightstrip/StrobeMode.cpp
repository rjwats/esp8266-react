#include <lightstrip/StrobeMode.h>

void StrobeMode::enable() {
  strobeState = 0;
  nextStrobe = millis();
}

void StrobeMode::tick() {
  long now = millis();
  if (nextStrobe<=now){
    nextStrobe = now + map(SPEED_TABLE[strobeSpeed], 0, 255, STROBE_MAX_DURATION, STROBE_MIN_DURATION);
    if (strobeState){
      strobeState=0;
      strip->renderRGB(color, 0);
    }else{
      strobeState=1;
      strip->renderRGB(color, 255);
    }
  }
}

void StrobeMode::handleIrCode(uint64_t irCode, boolean repeat){
  // handle changing color
  if (!repeat && isColorCode(irCode)){
    CRGB newColor = getColorForCode(irCode);
    if (newColor != color){
      color= newColor;
    }
  }

  switch (irCode) {
    case IR_BPLUS: {
      uint8_t newStrobeSpeed = strobeSpeed + STROBE_SPEED_ADJUST;
      if (newStrobeSpeed < strobeSpeed){
        strobeSpeed = 255;
      }else{
        strobeSpeed = newStrobeSpeed;
      }
      break;
    }
    case IR_BMINUS: {
        uint8_t newStrobeSpeed = strobeSpeed - STROBE_SPEED_ADJUST;
        if (newStrobeSpeed > strobeSpeed){
          strobeSpeed = 0;
        }else{
          strobeSpeed = newStrobeSpeed;
        }
        break;
    }
  }
}

uint64_t StrobeMode::getModeCode() {
  return IR_STROBE;
}

void StrobeMode::updateConfig(JsonObject& root) {
  updateByteFromJson(root, &strobeSpeed, "strobe_speed");
  updateColorFromJson(root, &color);
}

void StrobeMode::writeConfig(JsonObject& root) {
  writeByteToJson(root, &strobeSpeed, "strobe_speed");
  writeColorToJson(root, &color);
}
