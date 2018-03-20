#include <Arduino.h>

#include <lightstrip/FlashMode.h>

void FlashMode::enable() {
  fade = 0;
  fadeFrom = &colorOne;
  fadeTo = &colorTwo;
  nextFade = millis();
}

void FlashMode::nblendU8TowardU8(uint8_t& cur, const uint8_t target, uint8_t amount) {
  if( cur == target) return;
  if( cur < target) {
    uint8_t delta = target - cur;
    delta = scale8_video(delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video(delta, amount);
    cur -= delta;
  }
}

CRGB FlashMode::calculateFadeColor() {
  CRGB calculatedColor = *fadeFrom;
  nblendU8TowardU8(calculatedColor.red, fadeTo->red, fade);
  nblendU8TowardU8(calculatedColor.green, fadeTo->green, fade);
  nblendU8TowardU8(calculatedColor.blue, fadeTo->blue, fade);
  return calculatedColor;
}

void FlashMode::tick() {
  long now = millis();
  if (nextFade<=now){
    if (fade==255){
      CRGB* oldFrom = fadeFrom;
      fadeFrom = fadeTo;
      fadeTo = oldFrom;
      fade = 0;
    } else {
      fade++;
    }
    strip->renderRGB(calculateFadeColor(), 255);
    nextFade = now + map(SPEED_TABLE[fadeSpeed], 0, 255, FADE_MAX_DELAY, FADE_MIN_DELAY);
  }
}

void FlashMode::handleIrCode(uint64_t irCode, boolean repeat){
  // handle changing color
  if (!repeat && isColorCode(irCode)){
    CRGB newColor = getColorForCode(irCode);
    colorTwo = colorOne;
    colorOne = newColor;
    enable();
  }

  // handle changing speed
  // TODO these functions needs to be helpers.. often needed!
  switch (irCode) {
    case IR_BPLUS: {
      if (fadeSpeed != 255) {
        uint8_t newFadeSpeed = fadeSpeed + FADE_SPEED_ADJUST;
        if (newFadeSpeed < fadeSpeed){
          fadeSpeed = 255;
        }else{
          fadeSpeed = newFadeSpeed;
        }
      }
      break;
    }
    case IR_BMINUS: {
      // TODO shared function should really take into account the top and bottom
      // and not apply the new speed if not required, returning boolean if the
      // value was modified.
      if (fadeSpeed != 0) {
        uint8_t newFadeSpeed = fadeSpeed - FADE_SPEED_ADJUST;
        if (newFadeSpeed > fadeSpeed){
          fadeSpeed = 0;
        }else{
          fadeSpeed = newFadeSpeed;
        }
        break;
      }
    }
  }
}

uint64_t FlashMode::getModeCode() {
  return IR_FLASH;
}

void FlashMode::updateConfig(JsonObject& root) {
  updateByteFromJson(root, &fadeSpeed, "fade_speed");
  updateColorFromJson(root, &colorOne, "color_one");
  updateColorFromJson(root, &colorTwo, "color_two");
}

void FlashMode::writeConfig(JsonObject& root) {
  writeByteToJson(root, &fadeSpeed, "fade_speed");
  writeColorToJson(root, &colorOne, "color_one");
  writeColorToJson(root, &colorTwo, "color_two");
}
