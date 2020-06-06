#include <RainbowEffect.h>

void RainbowEffect::activate() {
  _lastFrameMicros = micros();
}

void RainbowEffect::loop() {
  if (_state.rotateSpeed > 0) {
    unsigned long rotateDelayMicros = 1000000 / _state.rotateSpeed;
    unsigned long currentMicros = micros();
    unsigned long microsElapsed = (unsigned long)(currentMicros - _lastFrameMicros);
    if (microsElapsed >= rotateDelayMicros) {
      _lastFrameMicros = currentMicros;
      _initialhue++;
    }
  }
  fill_rainbow(LightEffect::_ledController->leds(), LightEffect::_ledController->size(), _initialhue, _state.hueDelta);
  FastLED.show();
}
