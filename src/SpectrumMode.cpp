#include <SpectrumMode.h>

String SpectrumMode::getId() {
  return "spectrum";
}

void SpectrumMode::enable() {
  _lastFrameMicros = micros();
  for (uint8_t i = 0; i < _numBands; i++) { 
    _peaks[i] = 0;
  }   
  _refresh = true;
}

void SpectrumMode::sampleComplete() {
  for (uint8_t i = 0; i < _numBands; i++) { 
     _peaks[i] = _max(_bands[i], _peaks[i]);
  }
}

void SpectrumMode::tick() {
  // rotate hue in time based manner
  // TODO - calculate based on time from max -> min rather than fixed per pixel.
  unsigned long rotateDelayMicros = 250000 / _decaySpeed;
  unsigned long currentMicros = micros();
  unsigned long microsElapsed = (unsigned long)(currentMicros - _lastFrameMicros);
  if (microsElapsed >= rotateDelayMicros){
    _lastFrameMicros = currentMicros;
    for (uint8_t i = 0; i < _numBands; i++) { 
      if (_peaks[i] > 0){
        _peaks[i] = _peaks[i] - 1;
      }
    }
  }  
  
  fill_solid(_leds, _numLeds, CRGB::Black);

  for (uint8_t i = 0; i < _numBands; i++) {
    // calculate ordinals for the average and peak
    uint16_t averageOrdinal = map(_bands[i], 0, ADC_MAX_VALUE, 0, _ledsPerChannel);
    uint16_t peakOrdinal = map(_peaks[i], 0, ADC_MAX_VALUE, 0, _ledsPerChannel);

    // draw main bar and the peak
    fill_solid(_leds + (i * _ledsPerChannel), averageOrdinal, _barColor);
    _leds[(i * _ledsPerChannel) + peakOrdinal] = _peakColor;
  }

  // refresh the led strip
  _ledController->showLeds(_brightness);
}

void SpectrumMode::readFromJsonObject(JsonObject &root) {
  updateColorFromJson(root, &_barColor, SPECTRUM_DEFAULT_BAR_COLOR, "bar_color"); 
  updateColorFromJson(root, &_peakColor, SPECTRUM_DEFAULT_PEAK_COLOR, "peak_color"); 
  updateByteFromJson(root, &_brightness, SPECTRUM_DEFAULT_BRIGHTNESS, "brightness");  
  updateByteFromJson(root, &_decaySpeed, SPECTRUM_DEFAULT_DECAY_SPEED, "decay_speed");
  _refresh = true;
}

void SpectrumMode::writeToJsonObject(JsonObject &root) {
  writeColorToJson(root, &_barColor, "bar_color");
  writeColorToJson(root, &_peakColor, "peak_color");   
  writeByteToJson(root, &_brightness, "brightness");
  writeByteToJson(root, &_decaySpeed, "decay_speed");
}
