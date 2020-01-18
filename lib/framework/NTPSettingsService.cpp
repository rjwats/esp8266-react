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
  settimeofday_cb(std::bind(&NTPSettingsService::receivedNTPtime, this));
}

NTPSettingsService::~NTPSettingsService() {
}

void NTPSettingsService::receivedNTPtime() {
  Serial.println("GOT TIME!");
}

void NTPSettingsService::loop() {
  // detect when we need to re-configure NTP and do it in the main loop
  if (_reconfigureNTP) {
    _reconfigureNTP = false;
    configureNTP();
  }
}

void NTPSettingsService::readFromJsonObject(JsonObject& root) {
  _server = root["server"] | NTP_SETTINGS_SERVICE_DEFAULT_SERVER;
  _tzLabel = root["tz_label"] | NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_LABEL;
  _tzFormat = root["tz_format"] | NTP_SETTINGS_SERVICE_DEFAULT_TIME_ZONE_FORMAT;
}

void NTPSettingsService::writeToJsonObject(JsonObject& root) {
  root["server"] = _server;
  root["tz_label"] = _tzLabel;
  root["tz_format"] = _tzFormat;
}

void NTPSettingsService::onConfigUpdated() {
  _reconfigureNTP = true;
}

#ifdef ESP32
void NTPSettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.printf("Got IP address, starting NTP Synchronization\n");
  _reconfigureNTP = true;
}

void NTPSettingsService::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.printf("WiFi connection dropped, stopping NTP.\n");
  _reconfigureNTP = false;
  sntp_stop();
}
#elif defined(ESP8266)
void NTPSettingsService::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.printf("Got IP address, starting NTP Synchronization\n");
  _reconfigureNTP = true;
}

void NTPSettingsService::onStationModeDisconnected(const WiFiEventStationModeDisconnected& event) {
  Serial.printf("WiFi connection dropped, stopping NTP.\n");
  _reconfigureNTP = false;
  sntp_stop();
}
#endif

void NTPSettingsService::configureNTP() {
  Serial.println("Configuring NTP...");

  // enable sync
  configTime(_tzFormat.c_str(), _server.c_str());
}
