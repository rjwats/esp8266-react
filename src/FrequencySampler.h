#ifndef FrequencySampler_h
#define FrequencySampler_h

#include <Arduino.h>
#include <StatefulService.h>
#include <FastLED.h>

#define FREQUENCY_SAMPLER_DEAD_ZONE 700
#define FREQUENCY_SAMPLER_RESET_PIN 4
#define FREQUENCY_SAMPLER_STROBE_PIN 5
#define FREQUENCY_SAMPLER_ANALOG_PIN 36

#define NUM_BANDS 7
#define ADC_MAX_VALUE 4096

class FrequencySamplerSettings {
 public:
  virtual float getSmoothingFactor() = 0;
};

class FrequencyData {
 public:
  uint16_t bands[NUM_BANDS];

  float calculateEnergyFloat(bool* includedBands = NULL) {
    uint16_t currentLevel = 0;
    uint16_t numBands = 0;
    for (uint16_t i = 0; i < NUM_BANDS; i++) {
      if (includedBands == NULL || includedBands[i]) {
        currentLevel += bands[i];
        numBands++;
      }
    }
    return (float)currentLevel / (numBands * ADC_MAX_VALUE);
  }

  uint8_t calculateEnergyByte(bool* includedBands = NULL) {
    return calculateEnergyFloat(includedBands) * 255;
  }

  static void read(FrequencyData& settings, JsonObject& root) {
    JsonArray array = root.createNestedArray("bands");
    for (uint16_t i = 0; i < NUM_BANDS; i++) {
      array.add(settings.bands[i]);
    }
  }
};

class FrequencySampler : public StatefulService<FrequencyData> {
 public:
  FrequencySampler(FrequencySamplerSettings* _settings);

  void begin();
  void loop();
  FrequencyData* getFrequencyData();

 private:
  FrequencySamplerSettings* _settings;
};

#endif  // end FrequencySampler_h
