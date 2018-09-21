#include <msgeq7/MSGEQ7SettingsService.h>

MSGEQ7SettingsService::MSGEQ7SettingsService(AsyncWebServer* server, FS* fs) : 
SettingsService(server, fs, MSGEQ7_SETTINGS_SERVICE_PATH, MSGEQ7_SETTINGS_FILE), 
_webSocket(MSGEQ7_FREQUENCY_STREAM) {
  _server->addHandler(&_webSocket);
}

MSGEQ7SettingsService::~MSGEQ7SettingsService() {
  _server->removeHandler(&_webSocket);
}

void MSGEQ7SettingsService::begin() {
	pinMode(MSGEQ7_RESET_PIN, OUTPUT);
	pinMode(MSGEQ7_STROBE_PIN, OUTPUT);
	pinMode(MSGEQ7_ANALOG_PIN, INPUT);  
}

void MSGEQ7SettingsService::loop() {
  unsigned long currentMillis = millis();
  unsigned long sampleElapsed = (unsigned long)(currentMillis - _lastSampledAt);
  if (sampleElapsed >= MSGEQ7_SAMPLE_DELAY_MS){
    _lastSampledAt = currentMillis;
    sampleNow();
  }
}

void MSGEQ7SettingsService::readFromJsonObject(JsonObject& root) {

}

void MSGEQ7SettingsService::writeToJsonObject(JsonObject& root) {

}

void MSGEQ7SettingsService::sampleNow() {
  // reset IC
  digitalWrite(MSGEQ7_STROBE_PIN, LOW);
	digitalWrite(MSGEQ7_RESET_PIN, HIGH);
	digitalWrite(MSGEQ7_RESET_PIN, LOW);

  for (uint8_t i = 0; i < 7; i++){
		// trigger each value in turn
		digitalWrite(MSGEQ7_STROBE_PIN, HIGH);
		digitalWrite(MSGEQ7_STROBE_PIN, LOW);

		// allow the output to settle
		delayMicroseconds(36);

    // read frequency for pin
    _samples[i] = analogRead(MSGEQ7_ANALOG_PIN);
  }

  transmitFrequencies();
}

void MSGEQ7SettingsService::transmitFrequencies() {
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
  }
}