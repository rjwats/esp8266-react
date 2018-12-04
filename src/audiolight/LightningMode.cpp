#include <audiolight/LightningMode.h>

String LightningMode::getId() {
  return "lightning";
}

void LightningMode::enable() {
  _refresh = true;
}

/**
* Analyzed threshold each time a sample is recieved
*/
void LightningMode::sampleComplete() {
  if (_state == State::IDLE) {
    if (calculateEnergyPercentage(_includedBands) >= _threshold) {
      // TODO: Can do a slightly better job of calculating this add some settings too
      _ledstart = random8(_numLeds);  // Determine starting location of flash
      _ledlen = random8(_numLeds-_ledstart);  // Determine length of flash (not to go beyond NUM_LEDS-1)

      _numFlashes = random8(3,_flashes); // Calculate the random number of flashes we will show
      _flashCounter = 0; // The number of flashes we have shown  

      // transition state to initialized
      _state = State::INITIALIZED;    
    }
  }
};

void LightningMode::tick() {
  if (_refresh){
    // assert idle mode
    _state = State::IDLE;

    // clear leds
    fill_solid(_leds,_numLeds,CHSV(255,0,0));
    _ledController->showLeds();

    // clear refresh flag
    _refresh = false;

    // eager return
    return;
  }

  if (_state == State::INITIALIZED || (_state == State::RUNNING && isWaitTimeElapsed())) {
    // assert running
    _state = State::RUNNING;

    // leader is scaled down by a factor of 5, return strokes are brighter
    _dimmer = (_flashCounter == 0 ) ? 5 : random8(1,3);

    // show flash
    fill_solid(_leds+_ledstart,_ledlen,_color);
    _ledController->showLeds(255/_dimmer);

    // wait a small amount of time (make this configurable? use microsecond timer to avoid delay?)
    delay(random8(4,10));

    // hide flash
    fill_solid(_leds+_ledstart,_ledlen,CHSV(255,0,0));
    _ledController->showLeds();
    
    // reset wait time for next flash
    resetWaitTime();

    // stop or wait
    if (++_flashCounter >= _numFlashes){
      _state = State::IDLE;
    }
  }
}


bool LightningMode::isWaitTimeElapsed() {
  unsigned long waitTimeElapsed = (unsigned long)(millis() - _waitStartedAt);
  return waitTimeElapsed >= _waitDuration;
}

void LightningMode::resetWaitTime() {
  _waitStartedAt = millis();
  // NB: leader is longer. TODO - config for this?
  _waitDuration = (_flashCounter==0 ? 200 : 50) + random8(100);
}

void LightningMode::updateConfig(JsonObject &root) {
  updateByteFromJson(root, &_threshold, LIGHTNING_DEFAULT_THRESHOLD, "threshold");
  updateByteFromJson(root, &_flashes, LIGHTNING_DEFAULT_FLASHES, "flashes");
  updateColorFromJson(root, &_color, LIGHTNING_DEFAULT_COLOR, "color");
  updateBooleanArrayFromJson(root, _includedBands, _numBands, "included_bands");

  if (_threshold > LIGHTNING_MAX_THRESHOLD){
    _threshold = LIGHTNING_DEFAULT_THRESHOLD;
  }

  // reset the mode
  _refresh = true;
}

void LightningMode::writeConfig(JsonObject &root) {
  writeByteToJson(root, &_threshold, "threshold"); 
  writeByteToJson(root, &_flashes, "flashes");  
  writeColorToJson(root, &_color, "color");
  writeBooleanArrayToJson(root, _includedBands, _numBands, "included_bands");
}
