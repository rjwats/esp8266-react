#include <audiolight/AudioLightSettingsService.h>

AudioLightSettingsService::AudioLightSettingsService(AsyncWebServer *server, FS *fs) : 
SettingsService(server, fs, AUDIO_LIGHT_SETTINGS_SERVICE_PATH, AUDIO_LIGHT_SETTINGS_FILE),
_webSocket(AUDIO_LIGHT_FREQUENCY_STREAM) {
  _server->addHandler(&_webSocket);
}

AudioLightSettingsService::~AudioLightSettingsService() {
  _server->removeHandler(&_webSocket);
}

void AudioLightSettingsService::begin() {
  pinMode(AUDIO_LIGHT_RESET_PIN, OUTPUT);
  pinMode(AUDIO_LIGHT_STROBE_PIN, OUTPUT);
  pinMode(AUDIO_LIGHT_ANALOG_PIN, INPUT);

  // TEMP - Set  Up FAST LED
  LEDS.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void AudioLightSettingsService::loop() {
  unsigned long currentMillis = millis();
  unsigned long sampleElapsed = (unsigned long)(currentMillis - _lastSampledAt);
  if (sampleElapsed >= AUDIO_LIGHT_SAMPLE_DELAY_MS) {
    _lastSampledAt = currentMillis;
    _numSamples ++;
    sampleNow();
  }
  unsigned long reportedElapsed = (unsigned long)(currentMillis - _lastReportedAt);
  if (reportedElapsed >= 1000) {
    Serial.print("Made ");
    Serial.print(_numSamples);
    Serial.println(" samples in 1 second.");      
    _lastReportedAt = currentMillis;
    _numSamples=0;
  }  
}

void AudioLightSettingsService::readFromJsonObject(JsonObject &root) {
}

void AudioLightSettingsService::writeToJsonObject(JsonObject &root) {
}

void AudioLightSettingsService::sampleNow() {
  // reset IC
  digitalWrite(AUDIO_LIGHT_STROBE_PIN, LOW);
  digitalWrite(AUDIO_LIGHT_RESET_PIN, HIGH);
  digitalWrite(AUDIO_LIGHT_RESET_PIN, LOW);

  for (uint8_t i = 0; i < 7; i++) {
    // trigger each value in turn
    digitalWrite(AUDIO_LIGHT_STROBE_PIN, HIGH);
    digitalWrite(AUDIO_LIGHT_STROBE_PIN, LOW);

    // allow the output to settle
    delayMicroseconds(36);

    // read frequency for pin
    _samples[i] = analogRead(AUDIO_LIGHT_ANALOG_PIN);
  }

  if (_samples[0] > 1000){
    makeLightning();
  }

  transmitFrequencies();
}

void  AudioLightSettingsService::makeLightning(){
  ledstart = random8(NUM_LEDS);                               // Determine starting location of flash
  ledlen = random8(NUM_LEDS-ledstart);                        // Determine length of flash (not to go beyond NUM_LEDS-1)
  
  for (int flashCounter = 0; flashCounter < random8(3,flashes); flashCounter++) {
    if(flashCounter == 0) dimmer = 5;                         // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(1,3);                               // return strokes are brighter than the leader
    
    fill_solid(leds+ledstart,ledlen,CHSV(255, 0, 255/dimmer));
    FastLED.show();                       // Show a section of LED's
    delay(random8(4,10));                                     // each flash only lasts 4-10 milliseconds
    fill_solid(leds+ledstart,ledlen,CHSV(255,0,0));           // Clear the section of LED's
    FastLED.show();
    
    if (flashCounter == 0) delay (150);                       // longer delay until next flash after the leader
    
    delay(50+random8(100));                                   // shorter delay between strokes  
  } // for()
}

void AudioLightSettingsService::transmitFrequencies() {
  if (_webSocket.count() == 0) {
    return;
  }

  uint8_t length = sprintf(
      _outputBuffer, "[%d,%d,%d,%d,%d,%d,%d]",
      _samples[0],
      _samples[1],
      _samples[2],
      _samples[3],
      _samples[4],
      _samples[5],
      _samples[6]);

  _webSocket.textAll(_outputBuffer, length);  
}

  /*
  // build JSON
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& frequencies = root.createNestedArray("frequencies");
  for (uint8_t i = 0; i < 7; i++){
    frequencies.add(_samples[i]);
  }

  // send to all clients
  size_t len = root.measureLength();
  AsyncWebSocketMessageBuffer * buffer = _webSocket.makeBuffer(len);
  if (buffer) {
    root.printTo((char *)buffer->get(), len + 1);
    _webSocket.textAll(buffer);
  }*/