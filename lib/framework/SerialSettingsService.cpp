#include "SerialSettingsService.h"

SerialSettingsService::SerialSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    httpEndpoint(SerialSettings::read, SerialSettings::update, this, server, SERIAL_SETTINGS_SERVICE_PATH, securityManager),
    fsPersistence(SerialSettings::read, SerialSettings::update, this, fs, SERIAL_SETTINGS_FILE) {
        addUpdateHandler([&](const String& originId) { configureSerial(); }, false);
}

void SerialSettingsService::begin() {
  this->fsPersistence.readFromFS();
  configureSerial();
  Serial.println("Stopping ser2net server");
  this->tcpServer = StreamServer{&this->serial};
  this->tcpServer.setup();
}

void SerialSettingsService::loop() {
    if(this->_state.enabled) {
        this->tcpServer.loop();
    }
}

void SerialSettingsService::end() {
  Serial.println("Stopping ser2net server");
  this->tcpServer.end();
  this->serial.end();
}

void SerialSettingsService::configureSerial() {
  // disconnect if currently connected
  this->end();
  // only connect if Serial is enabled
  if (this->_state.enabled) {
    Serial.println("Starting serial with pins ");
    this->serial.begin(this->_state.baud, this->_state.config, this->_state.rxPin, this->_state.txPin, this->_state.invert);
    Serial.printf("Starting tcp server on port %u\n", this->_state.tCPPort);
    this->tcpServer.set_port(this->_state.tCPPort);
    this->tcpServer.setup();
  }
}

