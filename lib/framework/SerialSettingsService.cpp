#include "SerialSettingsService.h"

SerialSettingsService::SerialSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    httpEndpoint(SerialSettings::read, SerialSettings::update, this, server, SERIAL_SETTINGS_SERVICE_PATH, securityManager),
    fsPersistence(SerialSettings::read, SerialSettings::update, this, fs, SERIAL_SETTINGS_FILE) {
        addUpdateHandler([&](const String& originId) { configureSerial(); }, false);
}

void SerialSettingsService::begin() {
  fsPersistence.readFromFS();
  configureSerial();
  Serial.println("Stopping ser2net server");
  _tcpServer = StreamServer{&_serial};
  _tcpServer.setup();
}

void SerialSettingsService::loop() {
    if(_state.enabled) {
        _tcpServer.loop();
    }
}

void SerialSettingsService::end() {
  Serial.println("Stopping ser2net server");
  _tcpServer.end();
  _serial.end();
}

void SerialSettingsService::configureSerial() {
  // disconnect if currently connected
  end();
  // only connect if Serial is enabled
  if (_state.enabled) {
    Serial.printf("Starting serial with rx pin %u and tx pin %u at %u baud\n",_state.rxPin, _state.txPin, _state.baud);
    _serial.begin(_state.baud, _state.config, _state.rxPin, _state.txPin, _state.invert);
    Serial.printf("Starting tcp server on port %u\n", _state.tCPPort);
    _tcpServer.set_port(_state.tCPPort);
    _tcpServer.setup();
  }
}

