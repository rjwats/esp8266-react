#include "SerialSettingsService.h"

SerialSettingsService::SerialSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    httpEndpoint(SerialSettings::read, SerialSettings::update, this, server, SERIAL_SETTINGS_SERVICE_PATH, securityManager),
    fsPersistence(SerialSettings::read, SerialSettings::update, this, fs, SERIAL_SETTINGS_FILE) {
        addUpdateHandler([&](const String& originId) { configureSerial(); }, false);
}

void SerialSettingsService::begin() {
  this->fsPersistence.readFromFS();
  configureSerial();
  this->tcpServer = StreamServer{this->serialServer.connect("tcpServer")};
  this->tcpServer.setup();
}

void SerialSettingsService::loop() {
    if(this->_state.enabled) {
        this->serialServer.loop();
        this->tcpServer.loop();
    }
}

void SerialSettingsService::configureSerial() {
  // disconnect if currently connected
  this->serialServer.end();
    this->serialServer.setBaud(this->_state.baud);
    this->serialServer.setRxPin(this->_state.rxPin);
    this->serialServer.setTxPin(this->_state.txPin);
    this->serialServer.setInvert(this->_state.invert);
    this->serialServer.setConfig(this->_state.config);

  // only connect if Serial is enabled
  if (this->_state.enabled) {
    this->serialServer.begin();
  }
}

