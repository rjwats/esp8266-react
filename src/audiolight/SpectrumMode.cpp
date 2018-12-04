#include <audiolight/SpectrumMode.h>

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
     _peaks[i] = max(_bands[i], _peaks[i]);
  }
}

void SpectrumMode::tick() {

  // rotate hue in time based manner
  unsigned long rotateDelayMicros = 500000 / 255;
  unsigned long currentMicros = micros();
  unsigned long microsElapsed = (unsigned long)(currentMicros - _lastFrameMicros);
  if (microsElapsed >= rotateDelayMicros){
    _lastFrameMicros = currentMicros;
    for (uint8_t i = 0; i < _numBands; i++) { 
      _peaks[i] = _peaks[i] - 1;
    }
  }  
  /*
  EVERY_N_MILLIS(20) {
    if (refresh){
      
      Serial.print("Set decay ammount to ");
      Serial.println(_peakDecayAmount);
      _refresh = false;
    }
  }
  */
 // decay the peak
  /*
  for (uint8_t i = 0; i < _numBands; i++) { 
     _peaks[i] = max(_bands[i], _peaks[i]);
  }*/

  
  fill_solid(_leds, _numLeds, CRGB::Black);

  for (uint8_t i = 0; i < _numBands; i++) {
    // calculate ordinals for the average and peak
    uint16_t averageOrdinal = map(_bands[i], 0, ADC_MAX_VALUE, 0, _ledsPerChannel);
    uint16_t peakOrdinal = map(_peaks[i], 0, ADC_MAX_VALUE, 0, _ledsPerChannel);

    // draw main bar and the peak
    fill_solid(_leds + (i * _ledsPerChannel), averageOrdinal, CRGB::Blue);
    _leds[(i * _ledsPerChannel) + peakOrdinal] = 0x440000;
  }

  // refresh the led strip
  _ledController->showLeds(_brightness);

}

void SpectrumMode::updateConfig(JsonObject &root) {
  updateByteFromJson(root, &_brightness, SPECTRUM_DEFAULT_BRIGHTNESS, "brightness");
  _refresh = true;
}

void SpectrumMode::writeConfig(JsonObject &root) {
  writeByteToJson(root, &_brightness, "brightness");
}
