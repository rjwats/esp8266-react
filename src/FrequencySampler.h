#ifndef FrequencySampler_h
#define FrequencySampler_h

#include <StatefulService.h>
#include <FastLED.h>
#include <Arduino.h>

#define FREQUENCY_SAMPLER_DEAD_ZONE 700
#define FREQUENCY_SAMPLER_RESET_PIN 4
#define FREQUENCY_SAMPLER_STROBE_PIN 5
#define FREQUENCY_SAMPLER_ANALOG_PIN 36
#define FREQUENCY_SAMPLER_DEFAULT_ROLLING_AVG_FACTOR 0.15

#define NUM_BANDS 7
#define ADC_MAX_VALUE 4096

class FrequencyData {
 public:
  uint16_t bands[NUM_BANDS];
  uint16_t rolling[NUM_BANDS];
};

class FrequencySampler : public StatefulService<FrequencyData> {
 public:
  void begin();
  void loop();

 private:
  float _rollingAverageFactor = FREQUENCY_SAMPLER_DEFAULT_ROLLING_AVG_FACTOR;
};

#endif  // end FrequencySampler_h
