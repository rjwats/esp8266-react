#include <LightningMode.h>

LightningMode::LightningMode(AsyncWebServer* server,
                             FS* fs,
                             SecurityManager* securityManager,
                             LedSettingsService* ledSettingsService,
                             FrequencySampler* frequencySampler) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
                       ledSettingsService,
                       frequencySampler,
                       LightningModeSettings::read,
                       LightningModeSettings::update,
                       LIGHTNING_MODE_ID) {
  addUpdateHandler([&](const String& originId) { enable(); }, false);
};

void LightningMode::tick() {
  _ledSettingsService->update([&](CRGB* leds, CLEDController* ledController, const uint16_t numLeds) {
    if (_refresh) {
      _status = Status::IDLE;                      // assert idle mode
      fill_solid(leds, numLeds, CHSV(255, 0, 0));  // clear leds
      ledController->showLeds();                   // render all leds black
      _refresh = false;                            // clear refresh flag
      return;                                      // eager return
    }

    if (_status == Status::TRIGGERED) {
      _ledstart = random8(numLeds);              // Determine starting location of flash
      _ledlen = random8(numLeds - _ledstart);    // Determine length of flash (not to go beyond NUM_LEDS-1)
      _numFlashes = random8(3, _state.flashes);  // Calculate the random number of flashes we will show
      _flashCounter = 0;                         // The number of flashes we have shown
      _status = Status::RUNNING;
    }

    if (_status == Status::RUNNING && isWaitTimeElapsed()) {
      _dimmer = (_flashCounter == 0) ? 5 : random8(1, 3);      // leader scaled by a 5, return strokes brighter
      fill_solid(leds + _ledstart, _ledlen, _state.color);     // draw the flash
      ledController->showLeds(_state.brightness / _dimmer);    // show the flash
      delay(random8(4, 10));                                   // wait a small amount of time (use non blocking delay?)
      fill_solid(leds + _ledstart, _ledlen, CHSV(255, 0, 0));  // hide flash
      ledController->showLeds();                               // draw hidden leds
      resetWaitTime();                                         // reset wait time for next flash

      // decrement flash counter, and reset to idle if done
      if (++_flashCounter >= _numFlashes) {
        _status = Status::IDLE;
      }
    }
  });
}

void LightningMode::enable() {
  _refresh = true;
}

void LightningMode::sampleComplete() {
  if (_status == Status::IDLE) {
    FrequencyData* frequencyData = _frequencySampler->getFrequencyData();
    if (frequencyData->calculateEnergyByte(_state.includedBands) >= _state.threshold) {
      _status = Status::TRIGGERED;
    }
  }
}

bool LightningMode::isWaitTimeElapsed() {
  unsigned long waitTimeElapsed = (unsigned long)(millis() - _waitStartedAt);
  return waitTimeElapsed >= _waitDuration;
}

void LightningMode::resetWaitTime() {
  _waitStartedAt = millis();                                       // mark current time for the non blocking delay
  _waitDuration = (_flashCounter == 0 ? 200 : 50) + random8(100);  // set wait, lead flash longer (make configurable)
}
