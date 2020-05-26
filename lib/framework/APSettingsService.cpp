#include <APSettingsService.h>

APSettingsService::APSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(APSettings::serialize,
                  APSettings::deserialize,
                  this,
                  server,
                  AP_SETTINGS_SERVICE_PATH,
                  securityManager),
    _fsPersistence(APSettings::serialize, APSettings::deserialize, this, fs, AP_SETTINGS_FILE),
    _dnsServer(nullptr),
    _lastManaged(0) {
  addUpdateHandler([&](const String& originId) { reconfigureAP(); }, false);
}

void APSettingsService::begin() {
  _fsPersistence.readFromFS();
  reconfigureAP();
}

void APSettingsService::reconfigureAP() {
  _lastManaged = millis() - MANAGE_NETWORK_DELAY;
}

void APSettingsService::loop() {
  unsigned long currentMillis = millis();
  unsigned long manageElapsed = (unsigned long)(currentMillis - _lastManaged);
  if (manageElapsed >= MANAGE_NETWORK_DELAY) {
    _lastManaged = currentMillis;
    manageAP();
  }
  handleDNS();
}

void APSettingsService::manageAP() {
  WiFiMode_t currentWiFiMode = WiFi.getMode();
  APSettings state = getStateSnapshot();
  if (state.provisionMode == AP_MODE_ALWAYS ||
      (state.provisionMode == AP_MODE_DISCONNECTED && WiFi.status() != WL_CONNECTED)) {
    if (currentWiFiMode == WIFI_OFF || currentWiFiMode == WIFI_STA) {
      startAP();
    }
  } else if ((currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA) 
      && WiFi.softAPgetStationNum() == 0) {
    stopAP();
  }
}

void APSettingsService::startAP() {
  Serial.println(F("Starting software access point"));
  APSettings state = getStateSnapshot();
  // WiFi.softAP owns the copy of ssid and passwords strings
  WiFi.softAP(state.ssid.c_str(), state.password.c_str());
  if (!_dnsServer) {
    IPAddress apIp = WiFi.softAPIP();
    Serial.print(F("Starting captive portal on "));
    Serial.println(apIp);
    _dnsServer = new DNSServer;
    _dnsServer->start(DNS_PORT, "*", apIp);
  }
}

void APSettingsService::stopAP() {
  if (_dnsServer) {
    Serial.println(F("Stopping captive portal"));
    _dnsServer->stop();
    delete _dnsServer;
    _dnsServer = nullptr;
  }
  Serial.println(F("Stopping software access point"));
  WiFi.softAPdisconnect(true);
}

void APSettingsService::handleDNS() {
  if (_dnsServer) {
    _dnsServer->processNextRequest();
  }
}

APSettings APSettingsService::getStateSnapshot() { 
  beginTransaction();
  APSettings snapshot(_state);
  endTransaction();
  return snapshot; 
}