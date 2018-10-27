#include <audiolight/AudioLightSettingsService.h>

AudioLightSettingsService::AudioLightSettingsService(AsyncWebServer *server, FS *fs) :
SimpleService(server, AUDIO_LIGHT_SERVICE_PATH),SimpleSocket(server, AUDIO_LIGHT_WS_PATH), 
_server(server), _webSocket(AUDIO_LIGHT_FREQUENCY_STREAM) {
  _server->addHandler(&_webSocket);

  _ledController = &FastLED.addLeds<LED_TYPE,LED_DATA_PIN,COLOR_ORDER>(_leds, NUM_LEDS);
  
  _modes[0] = new OffMode(_ledController, _leds, NUM_LEDS, _frequencies);
  _modes[1] = new ColorMode(_ledController, _leds, NUM_LEDS, _frequencies);
  _modes[2] = new SpectrumMode(_ledController, _leds, NUM_LEDS, _frequencies);
  _modes[3] = new RainbowMode(_ledController, _leds, NUM_LEDS, _frequencies);
  _modes[4] = new LightningMode(_ledController, _leds, NUM_LEDS, _frequencies);
  
  // off mode is default
  _currentMode = _modes[1];
  _currentMode->enable();

  _audioFFT.begin();
}

AudioLightSettingsService::~AudioLightSettingsService() {
  _server->removeHandler(&_webSocket);
}

void AudioLightSettingsService::begin() {
  pinMode(AUDIO_LIGHT_RESET_PIN, OUTPUT);
  pinMode(AUDIO_LIGHT_STROBE_PIN, OUTPUT);
  pinMode(AUDIO_LIGHT_ANALOG_PIN, INPUT);
  digitalWrite(AUDIO_LIGHT_RESET_PIN, LOW);
  digitalWrite(AUDIO_LIGHT_STROBE_PIN, HIGH);  
}

void AudioLightSettingsService::loop() {
  unsigned long currentMicros = micros();
  unsigned long tickElapsed = (unsigned long)(currentMicros - _lastTickAt);
  if (tickElapsed >= AUDIO_LIGHT_TICK_DELAY_MICROS) {
    _lastTickAt = currentMicros;
    _numSamples ++;
    tick();
  }
  unsigned long reportedElapsed = (unsigned long)(currentMicros - _lastReportedAt);
  if (reportedElapsed >= 1000000) {
    Serial.print("Made ");
    Serial.print(_numSamples);
    Serial.println(" samples in 1 second.");      
    _lastReportedAt = currentMicros;
    _numSamples=0;
  }  
}

AudioLightMode* AudioLightSettingsService::getMode(String modeId) {
  for (uint8_t i = 0; i<NUM_MODES; i++){
    AudioLightMode* mode = _modes[i];
    if (mode->getId() == modeId){
      return mode;
    }
  }
  return NULL;
}

void AudioLightSettingsService::readFromJsonObject(JsonObject& root, String originId) {
  // update the config
  String modeId = root["mode_id"];

  // get mode we are configuring
  AudioLightMode *mode = getMode(modeId);

  // switch mode
  if (mode != NULL){
    mode->updateConfig(root);
    if (mode != _currentMode){
      _currentMode = mode;
      _currentMode->enable();
    }
  }

  // push the updates out to the WebSockets
  pushPayloadToWebSockets(originId);
}

void AudioLightSettingsService::writeToJsonObject(JsonObject& root) {
  root["mode_id"] = _currentMode->getId();
  _currentMode->writeConfig(root);
}

void AudioLightSettingsService::tick() {
  /*
  // Reset MSGEQ7 IC
  digitalWrite(AUDIO_LIGHT_RESET_PIN, HIGH);
  digitalWrite(AUDIO_LIGHT_RESET_PIN, LOW);

  // read samples
  for (uint8_t i = 0; i < 7; i++) {
    // trigger each value in turn
    digitalWrite(AUDIO_LIGHT_STROBE_PIN, LOW);

    // allow the output to settle
    delayMicroseconds(36);

    // read frequency for pin
    _frequencies[i] = analogRead(AUDIO_LIGHT_ANALOG_PIN);

    // re-map frequency to eliminate low level noise
    _frequencies[i]  = _frequencies[i] > 125 ? map(_frequencies[i]-125, 0, 1024-125, 0, 1024) : 0;

    // strobe pin high again for next loop
    digitalWrite(AUDIO_LIGHT_STROBE_PIN, HIGH);
  }
*/
  
  // sample
  _audioFFT.sampleAndFFT();

  //_frequencies[0] = _adc.sample(_adc.CH0);
  //_frequencies[1] = _adc.sample(_adc.CH1);

  // transmit frequencies over web sockets if possible
  transmitFrequencies();

  // allow current mode to adjust the leds
  _currentMode->tick();
}

void AudioLightSettingsService::transmitFrequencies() {
  if (_webSocket.count() == 0) {
    return;
  }

  uint8_t length = sprintf(
      _outputBuffer, "[%d,%d,%d,%d,%d,%d,%d]",
      _frequencies[0],
      _frequencies[1],
      _frequencies[2],
      _frequencies[3],
      _frequencies[4],
      _frequencies[5],
      _frequencies[6]);

  _webSocket.textAll(_outputBuffer, length);  
}
