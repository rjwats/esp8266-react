#include <FireMode.h>

String FireMode::getId() {
  return "fire";
}

void FireMode::enable() {
  _refresh = true;
}

/**
* Analyzed threshold each time a sample is recieved
*/
void FireMode::sampleComplete() {};

void FireMode::tick() {
  if (_refresh){
    // clear leds
    fill_solid(_leds,_numLeds,CHSV(255,0,0));
    _ledController->showLeds();

    Serial.println("clear");

    // clear heat map
    memset(_heatMap, 0, sizeof(uint8_t) * _numLeds);

    // clear refresh flag
    _refresh = false;
  }
  
  // make firey stuff at 100FPS
  EVERY_N_MILLIS(10) {
    // Step 1.  Cool down every cell a little
    for( int i = 0; i < _numLeds; i++) {
      _heatMap[i] = qsub8( _heatMap[i],  random8(0, ((_cooling * 10) / _numLeds) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= _numLeds - 1; k >= 2; k--) {
      _heatMap[k] = (_heatMap[k - 1] + _heatMap[k - 2] + _heatMap[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < _sparking ) {
      int y = random8(7);
      _heatMap[y] = qadd8( _heatMap[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < _numLeds; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( _heatMap[j], 240);
      CRGB color = ColorFromPalette( *_firePalette, colorindex);
      int pixelnumber;
      if( _reverse ) {
        pixelnumber = (_numLeds-1) - j;
      } else {
        pixelnumber = j;
      }
      _leds[pixelnumber] = color;
    }
    _ledController->showLeds();
  }

}

void FireMode::readFromJsonObject(JsonObject &root) {
  // reset the mode
  _refresh = true;
}

void FireMode::writeToJsonObject(JsonObject &root) {

}
