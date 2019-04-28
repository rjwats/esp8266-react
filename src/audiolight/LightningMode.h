#ifndef LIGHTNING_MODE_H
#define LIGHTNING_MODE_H

#include <Arduino.h>
#include <FastLed.h>

#include <audiolight/AudioLightMode.h>
#include <audiolight/JsonUtil.h>

#define LIGHTNING_DEFAULT_COLOR CRGB::White
#define LIGHTNING_DEFAULT_FLASHES 8
#define LIGHTNING_DEFAULT_THRESHOLD 128
#define LIGHTNING_DEFAULT_BRIGHTNESS 255
#define LIGHTNING_FILE_PATH "/modes/lightning.json"

class LightningMode : public AudioLightMode {

private:
  enum class State { IDLE, INITIALIZED, RUNNING };

  // TODO - RENAME to "thresholdBands"
  // which channels are used to calculate the threshold to trigger the lightning?
  // we listen to the two bass channels by default - we are trying to detect thunder sounds
  bool *_includedBands;

  uint8_t _brightness = LIGHTNING_DEFAULT_BRIGHTNESS;
  
  uint8_t _threshold = LIGHTNING_DEFAULT_THRESHOLD; // initially we'll work with a simple % based threshold level for triggering a strike
                                                    // in the future this could be take into account the background noise to make
                                                    // sure a "significant" change is detected, which may change the settings
  //the upper limit of flashes per strike
  uint8_t _flashes = LIGHTNING_DEFAULT_FLASHES;    
  
  // What color, just in case we want funky colored lightning!
  CRGB _color = LIGHTNING_DEFAULT_COLOR;

  // State variables
  State _state = State::IDLE;
  uint8_t _ledstart; // Starting location of a flash
  uint8_t _ledlen; // Length of a flash
  uint16_t _dimmer = 1; // State for dimming initial flash by a factor
  uint8_t _flashCounter = 0;
  uint8_t _numFlashes = 0;
  bool _refresh = true;

  // pause tracking
  unsigned long _waitStartedAt;
  uint16_t _waitDuration;

public:
  LightningMode(FS* fs, CLEDController *ledController, CRGB *leds, uint16_t numLeds, uint16_t *bands,  uint16_t numBands)
      : AudioLightMode(fs, ledController, leds, numLeds, bands, numBands, LIGHTNING_FILE_PATH) {
    // TODO - Util for this, duplicated!
    _includedBands = (bool *) malloc(sizeof(bool) * numBands);
    for (int i=0; i<numBands / 2; i++) {
      _includedBands[i] = true;
    }
  };
  String getId();
  void tick();
  void sampleComplete();
  void enable();
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
  bool isWaitTimeElapsed();
  void resetWaitTime();
};

#endif
