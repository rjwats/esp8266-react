#include <FrequencySampler.h>

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
            frequencyData.bands[i] = analogRead(FREQUENCY_SAMPLER_ANALOG_PIN);

            // re-map frequency to eliminate low level noise
            frequencyData.bands[i] = frequencyData.bands[i] > FREQUENCY_SAMPLER_DEAD_ZONE
                                         ? map(frequencyData.bands[i] - FREQUENCY_SAMPLER_DEAD_ZONE,
                                               0,
                                               ADC_MAX_VALUE - FREQUENCY_SAMPLER_DEAD_ZONE,
                                               0,
                                               ADC_MAX_VALUE)
                                         : 0;

            // crappy rolling values
            frequencyData.rolling[i] =
                _rollingAverageFactor * frequencyData.bands[i] + (1 - _rollingAverageFactor) * frequencyData.rolling[i];

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
