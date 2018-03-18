#include <lightstrip/Strip.h>

Strip::Strip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin) {
   pinMode(redPin, OUTPUT);
   pinMode(greenPin, OUTPUT);
   pinMode(bluePin, OUTPUT);

   // store pins, we need them when rendering
   this->redPin=redPin;
   this->greenPin=greenPin;
   this->bluePin=bluePin;

   // This setting seems to fix the crashing issues related to low values:
   // https://github.com/esp8266/Arduino/issues/836
   analogWriteFreq(200);
}

void Strip::renderRGB(CRGB color, uint8_t brightness) {
  noInterrupts();
  analogWrite(redPin, LED_GAMMA_ADJUSTMENT[map(color.r, 0, 255, 0, brightness)]);
  analogWrite(greenPin, LED_GAMMA_ADJUSTMENT[map(color.g, 0, 255, 0, brightness)]);
  analogWrite(bluePin, LED_GAMMA_ADJUSTMENT[map(color.b, 0, 255, 0, brightness)]);
  interrupts();
}
