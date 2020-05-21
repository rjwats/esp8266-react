#include <NTPSettingsService.h>

NTPSettingsService::NTPSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(NTPSettings::serialize,
                  NTPSettings::deserialize,
                  this,
                  server,
                  NTP_SETTINGS_SERVICE_PATH,
                  securityManager),
    _fsPersistence(NTPSettings::serialize, NTPSettings::deserialize, this, fs, NTP_SETTINGS_FILE) {
#ifdef ESP32
  WiFi.onEvent(
      std::bind(&NTPSettingsService::onStationModeDisconnected, this, std::placeholders::_1, std::placeholders::_2),
      WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(std::bind(&NTPSettingsService::onStationModeGotIP, this, std::placeholders::_1, std::placeholders::_2),
               WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
#elif defined(ESP8266)
  _onStationModeDisconnectedHandler = WiFi.onStationModeDisconnected(
      std::bind(&NTPSettingsService::onStationModeDisconnected, this, std::placeholders::_1));
  _onStationModeGotIPHandler =
      WiFi.onStationModeGotIP(std::bind(&NTPSettingsService::onStationModeGotIP, this, std::placeholders::_1));
#endif
  addUpdateHandler([&](const String& originId) { configureNTP(); }, false);
}

void NTPSettingsService::begin() {
  _fsPersistence.readFromFS();
  configureNTP();
}

#ifdef ESP32
void NTPSettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println(F("Got IP address, starting NTP Synchronization"));
  configureNTP();
}

void NTPSettingsService::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println(F("WiFi connection dropped, stopping NTP."));
  configureNTP();
}
#elif defined(ESP8266)
void NTPSettingsService::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.println(F("Got IP address, starting NTP Synchronization"));
  configureNTP();
}

void NTPSettingsService::onStationModeDisconnected(const WiFiEventStationModeDisconnected& event) {
  Serial.println(F("WiFi connection dropped, stopping NTP."));
  configureNTP();
}
#endif

void NTPSettingsService::configureNTP() {
  if (WiFi.isConnected() && _state.enabled) {
    Serial.println(F("Starting NTP..."));
#ifdef ESP32
    configTzTime(_state.tzFormat.c_str(), _state.server.c_str());
#elif defined(ESP8266)
    configTime(_state.tzFormat.c_str(), _state.server.c_str());
#endif
  } else {
    sntp_stop();
  }
}
