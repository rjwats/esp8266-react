#include <audiolight/SpectrumMode.h>

String SpectrumMode::getId() {
  return "spectrum";
}

void SpectrumMode::enable() {
  _refresh = true;
}

void SpectrumMode::tick() {
  /*
  EVERY_N_MILLIS(20) {
    if (refresh){
      _peakDecayAmount = max(MIN_PEAK_DECAY_AMOUNT, (uint16_t)(((float)1000 / _peakDecayMs) * MAX_SAMPLE_SIZE) / 20);
      Serial.print("Set decay ammount to ");
      Serial.println(_peakDecayAmount);
      _refresh = false;
    }
  }
  */

  // refresh
  for (uint8_t i = 0; i < _numBands; i++) {
    // use approx approach for rolling averages
    _rollingAverages[i] = _rollingAverageFactor * _bands[i] + (1 - _rollingAverageFactor) * _rollingAverages[i];

     _peaks[i] = (_peaks[i] > _peakDecayAmount) ? _peaks[i] - _peakDecayAmount : 0;     
     _peaks[i] = max(_rollingAverages[i], _peaks[i]);
  }

  // TEMP - Display averages and peaks on "equalizer" LEDs
  // TODO - calculate leds per channel once only...
  uint16_t ledsPerChannel = _numLeds / _numBands;
  fill_solid(_leds, _numLeds, CRGB::Black);

  for (uint8_t i = 0; i < _numBands; i++) {
    // calculate ordinals for the average and peak
    uint16_t averageOrdinal = map(_rollingAverages[i], 0, ADC_MAX_VALUE, 0, ledsPerChannel);
    uint16_t peakOrdinal = map(_peaks[i], 0, ADC_MAX_VALUE, 0, ledsPerChannel);

    // draw the leds, red over blue if red=blue
    //_leds[(i * ledsPerChannel) + averageOrdinal] = CRGB::Blue;

    fill_solid(_leds + (i * ledsPerChannel), averageOrdinal, CRGB::Blue);

    _leds[(i * ledsPerChannel) + peakOrdinal] = 0x440000;    
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
