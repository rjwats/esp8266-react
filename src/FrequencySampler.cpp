#include <FrequencySampler.h>

FrequencySampler::FrequencySampler(LedSettingsService* ledSettingsService) : _ledSettingsService(ledSettingsService) {
  ledSettingsService->addUpdateHandler([&](const String& originId) { updateSettings(); }, false);
}

void FrequencySampler::begin() {
  pinMode(FREQUENCY_SAMPLER_RESET_PIN, OUTPUT);
  pinMode(FREQUENCY_SAMPLER_STROBE_PIN, OUTPUT);
  pinMode(FREQUENCY_SAMPLER_ANALOG_PIN, INPUT);
  digitalWrite(FREQUENCY_SAMPLER_RESET_PIN, LOW);
  digitalWrite(FREQUENCY_SAMPLER_STROBE_PIN, HIGH);
  updateSettings();
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
            uint16_t value = analogRead(FREQUENCY_SAMPLER_ANALOG_PIN);

            // re-map frequency to eliminate low level noise
            value = value > _deadZone ? map(value - _deadZone, 0, ADC_MAX_VALUE - _deadZone, 0, ADC_MAX_VALUE) : 0;

            // crappy smoothing to avoid crazy flickering
            frequencyData.bands[i] = _smoothingFactor * frequencyData.bands[i] + (1 - _smoothingFactor) * value;

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

void FrequencySampler::updateSettings() {
  _ledSettingsService->read([&](LedSettings& ledSettings) {
    _smoothingFactor = ledSettings.smoothingFactor;
    _deadZone = ledSettings.deadZone;
  });
}

FrequencyData* FrequencySampler::getFrequencyData() {
  return &_state;
}
