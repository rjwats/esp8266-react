#include <audiolight/SpectrumMode.h>

String SpectrumMode::getId() {
  return "flashy_beats";
}

void SpectrumMode::enable() {
  _refresh = true;
}

void SpectrumMode::tick() {
  if (_refresh) {
    _peakDecayAmount = max(MIN_PEAK_DECAY_AMOUNT, MAX_SAMPLE_SIZE / _peakDecayMs);
    Serial.print("Set decay ammount to ");
    Serial.println(_peakDecayAmount);
    _refresh = false;
  }

  // refresh
  for (uint8_t i = 0; i < 7; i++) {
    // use approx approach for rolling averages
    _rollingAverages[i] = _rollingAverageFactor * _frequencies[i] + (1 - _rollingAverageFactor) * _rollingAverages[i];

    // use whatever is larger, decayed value
    uint16_t decayedValue = _peaks[i] - _peakDecayAmount;
    _peaks[i] = max(_frequencies[i], decayedValue);
  }

  // TEMP - Display averages and peaks on "equalizer" LEDs
  // TODO - calculate leds per channel once only...
  uint16_t ledsPerChannel = _numLeds / 7;
  fill_solid(_leds, _numLeds, CRGB::Black);

  for (uint8_t i = 0; i < 7; i++) {
    // calculate ordinals for the average and peak
    uint16_t averageOrdinal = map(_rollingAverages[i], 0, 1024, 0, ledsPerChannel);
    uint16_t peakOrdinal = map(_peaks[i], 0, 1024, 0, ledsPerChannel);

    // draw the leds, red over blue if red=blue
    _leds[(i * ledsPerChannel) + averageOrdinal] = CRGB::Blue;
    _leds[(i * ledsPerChannel) + peakOrdinal] = CRGB::Red;    
  }

  // refresh the led strip
  _ledController->showLeds(_brightness);
}

void SpectrumMode::updateConfig(JsonObject &root) {
  updateByteFromJson(root, &_brightness, "brightness");
  _refresh = true;
}

void SpectrumMode::writeConfig(JsonObject &root) {
  writeByteToJson(root, &_brightness, "brightness");
}
