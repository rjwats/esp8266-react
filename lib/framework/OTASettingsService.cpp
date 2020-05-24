#include <OTASettingsService.h>

OTASettingsService::OTASettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(OTASettings::serialize,
                  OTASettings::deserialize,
                  this,
                  server,
                  OTA_SETTINGS_SERVICE_PATH,
                  securityManager),
    _fsPersistence(OTASettings::serialize, OTASettings::deserialize, this, fs, OTA_SETTINGS_FILE),
    _arduinoOTA(nullptr) {
#ifdef ESP32
  WiFi.onEvent(std::bind(&OTASettingsService::onStationModeGotIP, this, std::placeholders::_1, std::placeholders::_2),
               WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
#elif defined(ESP8266)
  _onStationModeGotIPHandler =
      WiFi.onStationModeGotIP(std::bind(&OTASettingsService::onStationModeGotIP, this, std::placeholders::_1));
#endif
  addUpdateHandler([&](const String& originId) { configureArduinoOTA(); }, false);
}

void OTASettingsService::begin() {
  _fsPersistence.readFromFS();
  configureArduinoOTA();
}

void OTASettingsService::loop() {
  if (_state.enabled && _arduinoOTA) {
    _arduinoOTA->handle();
  }
}

static const char* otaErrorMessage(ota_error_t error) {
  switch (error) {
    case OTA_AUTH_ERROR:
      return PSTR("Authentication failed");
    case OTA_BEGIN_ERROR:
      return PSTR("Begin failed");
    case OTA_CONNECT_ERROR:
      return PSTR("Connect failed");
    case OTA_RECEIVE_ERROR:
      return PSTR("Receive failed");
    case OTA_END_ERROR:
      return PSTR("End failed");
    default:
      return PSTR("Unknown error");
  }
}

void OTASettingsService::configureArduinoOTA() {
  if (_arduinoOTA) {
#ifdef ESP32
    _arduinoOTA->end();
#endif
    delete _arduinoOTA;
    _arduinoOTA = nullptr;
  }
  if (_state.enabled) {
    LOG_I("Starting OTA update service...");
    _arduinoOTA = new ArduinoOTAClass;
    _arduinoOTA->setPort(_state.port);
    _arduinoOTA->setPassword(_state.password.c_str());
    _arduinoOTA->onStart([]() { LOG_I("Starting OTA update"); });
    _arduinoOTA->onEnd([]() { LOG_I("Ending OTA update"); });
    _arduinoOTA->onProgress([](unsigned int progress, unsigned int total) {
      LOGF_I("OTA update progress: %u%%", progress / (total / 100));
    });
    _arduinoOTA->onError(
        [](ota_error_t error) { LOGF_E("OTA update error[%u]: %s", error, otaErrorMessage(error)); });
    _arduinoOTA->begin();
  }
}

#ifdef ESP32
void OTASettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  configureArduinoOTA();
}
#elif defined(ESP8266)
void OTASettingsService::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  configureArduinoOTA();
}
#endif
