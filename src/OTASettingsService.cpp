#include <OTASettingsService.h>

OTASettingsService::OTASettingsService(AsyncWebServer* server, FS* fs) : SettingsService(server, fs, OTA_SETTINGS_SERVICE_PATH, OTA_SETTINGS_FILE) {
#if defined(ESP8266)
  _onStationModeGotIPHandler = WiFi.onStationModeGotIP(std::bind(&OTASettingsService::onStationModeGotIP, this, std::placeholders::_1));
#elif defined(ESP_PLATFORM)
  WiFi.onEvent(std::bind(&OTASettingsService::onStationModeGotIP, this, std::placeholders::_1, std::placeholders::_2), WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
#endif
}

OTASettingsService::~OTASettingsService() {}

void OTASettingsService::loop() {
  if (_enabled && _arduinoOTA){
    _arduinoOTA->handle();
  }
}

void OTASettingsService::onConfigUpdated() {
  configureArduinoOTA();
}

void OTASettingsService::readFromJsonObject(JsonObject& root) {
  _enabled = root["enabled"];
  _port = root["port"];
  _password = root["password"] | DEFAULT_OTA_PASSWORD;

  // provide defaults
  if (_port < 0){
    _port = DEFAULT_OTA_PORT;
  }
}

void OTASettingsService::writeToJsonObject(JsonObject& root) {
  root["enabled"] = _enabled;
  root["port"] = _port;
  root["password"] = _password;
}

void OTASettingsService::configureArduinoOTA() {
  if (_arduinoOTA){
    delete _arduinoOTA;
    _arduinoOTA = NULL;
  }
  if (_enabled) {
    Serial.println("Starting OTA Update Service");
    _arduinoOTA = new ArduinoOTAClass;
    _arduinoOTA->setPort(_port);
    _arduinoOTA->setPassword(_password.c_str());
    _arduinoOTA->onStart([]() {
      Serial.println("Starting");
    });
    _arduinoOTA->onEnd([]() {
      Serial.println("\nEnd");
    });
    _arduinoOTA->onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    _arduinoOTA->onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    _arduinoOTA->begin();
  }
}

#if defined(ESP8266)
void OTASettingsService::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  configureArduinoOTA();
}  
#elif defined(ESP_PLATFORM)
void OTASettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  configureArduinoOTA();
}  
#endif
