#include <FrequencySampler.h>

FrequencySampler::FrequencySampler(FrequencySamplerSettings* settings) : _settings(settings) {
}

void FrequencySampler::begin() {
  pinMode(FREQUENCY_SAMPLER_RESET_PIN, OUTPUT);
  pinMode(FREQUENCY_SAMPLER_STROBE_PIN, OUTPUT);
  pinMode(FREQUENCY_SAMPLER_ANALOG_PIN, INPUT);
  digitalWrite(FREQUENCY_SAMPLER_RESET_PIN, LOW);
  digitalWrite(FREQUENCY_SAMPLER_STROBE_PIN, HIGH);
}

void FrequencySampler::loop() {
  // take samples 100 times a second (max)
  EVERY_N_MILLIS(10) {
    float smoothingFactor = _settings->getSmoothingFactor();
    uint16_t deadZone = _settings->getDeadZone();
    update(
        [&](FrequencyData& frequencyData) {
          // Reset MSGEQ7 IC
          digitalWrite(FREQUENCY_SAMPLER_RESET_PIN, HIGH);
          delayMicroseconds(5);
          digitalWrite(FREQUENCY_SAMPLER_RESET_PIN, LOW);

          // read samples
          for (uint8_t i = 0; i < NUM_BANDS; i++) {
            // trigger each value in turn
            digitalWrite(FREQUENCY_SAMPLER_STROBE_PIN, LOW);

            // allow the output to settle
            delayMicroseconds(36);

            // read frequency for pin
            uint16_t value = analogRead(FREQUENCY_SAMPLER_ANALOG_PIN);

            // re-map frequency to eliminate low level noise
            value = value > deadZone ? map(value - deadZone, 0, ADC_MAX_VALUE - deadZone, 0, ADC_MAX_VALUE) : 0;

            // crappy smoothing to avoid crazy flickering
            frequencyData.bands[i] = smoothingFactor * frequencyData.bands[i] + (1 - smoothingFactor) * value;

            // strobe pin high again for next loop
            digitalWrite(FREQUENCY_SAMPLER_STROBE_PIN, HIGH);

            // wait 72 microseconds
            delayMicroseconds(36);
          }
          return StateUpdateResult::CHANGED;
        },
        "loop");
  }
}

FrequencyData* FrequencySampler::getFrequencyData() {
  return &_state;
}
