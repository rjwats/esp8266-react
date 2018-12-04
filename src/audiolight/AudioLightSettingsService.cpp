#include <audiolight/AudioLightSettingsService.h>

AudioLightSettingsService::AudioLightSettingsService(AsyncWebServer *server, FS *fs) :
SimpleService(server, AUDIO_LIGHT_SERVICE_PATH),SimpleSocket(server, AUDIO_LIGHT_WS_PATH), 
_server(server), _webSocket(AUDIO_LIGHT_FREQUENCY_STREAM) {
  _server->addHandler(&_webSocket);

  _ledController = &FastLED.addLeds<LED_TYPE,LED_DATA_PIN,COLOR_ORDER>(_leds, NUM_LEDS);
  
  _modes[0] = new OffMode(_ledController, _leds, NUM_LEDS, _bands, NUM_BANDS);
  _modes[1] = new ColorMode(_ledController, _leds, NUM_LEDS, _bands, NUM_BANDS);
  _modes[2] = new SpectrumMode(_ledController, _leds, NUM_LEDS, _bands, NUM_BANDS);
  _modes[3] = new RainbowMode(_ledController, _leds, NUM_LEDS, _bands, NUM_BANDS);
  _modes[4] = new LightningMode(_ledController, _leds, NUM_LEDS, _bands, NUM_BANDS);
  
  // off mode is default
  _currentMode = _modes[1];
  _currentMode->enable();
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
  // take samples 100 times a second (max)
  EVERY_N_MILLIS(10) {
    // Reset MSGEQ7 IC
    digitalWrite(AUDIO_LIGHT_RESET_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(AUDIO_LIGHT_RESET_PIN, LOW);

    // read samples
    for (uint8_t i = 0; i < NUM_BANDS; i++) {
      // trigger each value in turn
      digitalWrite(AUDIO_LIGHT_STROBE_PIN, LOW);

      // allow the output to settle
      delayMicroseconds(36);

      // read frequency for pin
      _bands[i] = analogRead(AUDIO_LIGHT_ANALOG_PIN);

      // re-map frequency to eliminate low level noise
      _bands[i]  =  _bands[i] > AUDIO_LIGHT_DEAD_ZONE ? map(_bands[i]-AUDIO_LIGHT_DEAD_ZONE, 0, ADC_MAX_VALUE-AUDIO_LIGHT_DEAD_ZONE, 0, ADC_MAX_VALUE) : 0;
      _rollingAverages[i] = _rollingAverageFactor * _bands[i] + (1 - _rollingAverageFactor) * _rollingAverages[i];

      // strobe pin high again for next loop
      digitalWrite(AUDIO_LIGHT_STROBE_PIN, HIGH);
    }
    
    // transmit over the wire - going to remove this.
    EVERY_N_MILLISECONDS(200){
      // transmit frequencies over web sockets if possible
      transmitFrequencies();
    }

    // notify sample taken
    _currentMode->sampleComplete();
  }
  // allow current mode to adjust the leds
  _currentMode->tick();
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

void AudioLightSettingsService::transmitFrequencies() {
  if (_webSocket.count() == 0) {
    return;
  }
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  for (uint16_t i = 0; i< NUM_BANDS; i++){
    array.add(_bands[i]);
  }
  size_t length = array.printTo(_outputBuffer, OUTPUT_BUFFER_SIZE);
  _webSocket.textAll(_outputBuffer, length);  
}
