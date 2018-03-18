#include <Arduino.h>

#include <lightstrip/FadeMode.h>

void FadeMode::enable() {
  fade = FADE_BY_MAX;
  fadeDirection = 0;
  nextFade = millis();
}

void FadeMode::tick() {
  long now = millis();
  if (nextFade<=now){
    if (fadeDirection){
      fade++;
      if (fade==FADE_BY_MAX){
        fadeDirection = 0;
      }
    }else{
      fade--;
      if (fade==0){
        fadeDirection = 1;
      }
    }
    CRGB nextColor = color;
    nextColor.fadeLightBy(fade);
    strip->renderRGB(nextColor, 255);
    nextFade = now + map(SPEED_TABLE[fadeSpeed], 0, 255, FADE_MAX_DELAY, FADE_MIN_DELAY);
  }
}

void FadeMode::handleIrCode(uint64_t irCode, boolean repeat){
  // handle changing color
  if (!repeat && isColorCode(irCode)){
    CRGB newColor = getColorForCode(irCode);
    if (newColor != color){
      color= newColor;
    }
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

uint64_t FadeMode::getModeCode() {
  return IR_FADE;
}

void FadeMode::updateConfig(JsonObject& root) {
  updateByteFromJson(root, &fadeSpeed, "fade_speed");
  updateColorFromJson(root, &color);
}

void FadeMode::writeConfig(JsonObject& root) {
  writeByteToJson(root, &fadeSpeed, "fade_speed");
  writeColorToJson(root, &color);
}
