#include <OTASettingsService.h>

static OTASettingsSerializer SERIALIZER;
static OTASettingsDeserializer DESERIALIZER;

OTASettingsService::OTASettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, OTA_SETTINGS_SERVICE_PATH, securityManager),
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, OTA_SETTINGS_FILE) {
#ifdef ESP32
  WiFi.onEvent(std::bind(&OTASettingsService::onStationModeGotIP, this, std::placeholders::_1, std::placeholders::_2),
               WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
#elif defined(ESP8266)
  _onStationModeGotIPHandler =
      WiFi.onStationModeGotIP(std::bind(&OTASettingsService::onStationModeGotIP, this, std::placeholders::_1));
#endif
  addUpdateHandler([&](void* origin) { configureArduinoOTA(); }, false);
}

void OTASettingsService::begin() {
  _settingsPersistence.readFromFS();
  configureArduinoOTA();
}

void OTASettingsService::loop() {
  if (_settings.enabled && _arduinoOTA) {
    _arduinoOTA->handle();
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
  if (_settings.enabled) {
    Serial.println("Starting OTA Update Service...");
    _arduinoOTA = new ArduinoOTAClass;
    _arduinoOTA->setPort(_settings.port);
    _arduinoOTA->setPassword(_settings.password.c_str());
    _arduinoOTA->onStart([]() { Serial.println("Starting"); });
    _arduinoOTA->onEnd([]() { Serial.println("\nEnd"); });
    _arduinoOTA->onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    _arduinoOTA->onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)
        Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR)
        Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR)
        Serial.println("End Failed");
    });
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
