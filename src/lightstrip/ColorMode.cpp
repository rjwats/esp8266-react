#include <lightstrip/ColorMode.h>

void ColorMode::enable() {
  refresh = true;
}

void ColorMode::tick() {
  if (refresh){
    strip->renderRGB(color, map(brightness, 0, 255, COLOR_MIN_BRIGHTNESS, 255));
    refresh = false;
  }
}

void ColorMode::handleIrCode(uint64_t irCode, boolean repeat){
  // handle changing color
  if (!repeat && isColorCode(irCode)){
    CRGB newColor = getColorForCode(irCode);
    if (newColor != color){
      color= newColor;
      refresh=true;
    }
  }

  // handle brightness change
  switch (irCode) {
    case IR_BPLUS: {
      uint8_t newBrightness = brightness + COLOR_BRIGHTNESS_ADJUST;
      if (newBrightness < brightness){
        brightness = 255;
      }else{
        brightness = newBrightness;
      }
      refresh = true;
      break;
    }
    case IR_BMINUS: {
        uint8_t newBrightness = brightness - COLOR_BRIGHTNESS_ADJUST;
        if (newBrightness > brightness){
          brightness = 0;
        }else{
          brightness = newBrightness;
        }
        refresh = true;
        break;
    }
  }
}

uint64_t ColorMode::getModeCode() {
  return IR_ON;
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
