#include <NTPSettingsService.h>

NTPSettingsService::NTPSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    AdminSettingsService(server, fs, securityManager, NTP_SETTINGS_SERVICE_PATH, NTP_SETTINGS_FILE) {
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
}

NTPSettingsService::~NTPSettingsService() {
}

void NTPSettingsService::loop() {
  // detect when we need to re-configure NTP and do it in the main loop
  if (_reconfigureNTP) {
    _reconfigureNTP = false;
    configureNTP();
  }
}

void NTPSettingsService::readFromJsonObject(JsonObject& root) {
  _settings.enabled = root["enabled"] | NTP_SETTINGS_SERVICE_DEFAULT_ENABLED;
  _settings.server = root["server"] | NTP_SETTINGS_SERVICE_DEFAULT_SERVER;
  _settings.tzLabel = root["tz_label"] | NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_LABEL;
  _settings.tzFormat = root["tz_format"] | NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_FORMAT;
}

void NTPSettingsService::writeToJsonObject(JsonObject& root) {
  root["enabled"] = _settings.enabled;
  root["server"] = _settings.server;
  root["tz_label"] = _settings.tzLabel;
  root["tz_format"] = _settings.tzFormat;
}

void NTPSettingsService::onConfigUpdated() {
  _reconfigureNTP = true;
}

#ifdef ESP32
void NTPSettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Got IP address, starting NTP Synchronization");
  _reconfigureNTP = true;
}

void NTPSettingsService::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connection dropped, stopping NTP.");
  _reconfigureNTP = false;
  sntp_stop();
}
#elif defined(ESP8266)
void NTPSettingsService::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.println("Got IP address, starting NTP Synchronization");
  _reconfigureNTP = true;
}

void NTPSettingsService::onStationModeDisconnected(const WiFiEventStationModeDisconnected& event) {
  Serial.println("WiFi connection dropped, stopping NTP.");
  _reconfigureNTP = false;
  sntp_stop();
}
#endif

void NTPSettingsService::configureNTP() {
  Serial.println("Configuring NTP...");
  if (_settings.enabled) {
#ifdef ESP32
    configTzTime(_settings.tzFormat.c_str(), _settings.server.c_str());
#elif defined(ESP8266)
    configTime(_settings.tzFormat.c_str(), _settings.server.c_str());
#endif
  } else {
    sntp_stop();
  }
}
