#include <APSettingsService.h>

static APSettingsSerializer SERIALIZER;
static APSettingsDeserializer DESERIALIZER;

APSettingsService::APSettingsService(FS* fs, AsyncWebServer* server, SecurityManager* securityManager) :
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, AP_SETTINGS_FILE),
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, AP_SETTINGS_SERVICE_PATH, securityManager) {
  addUpdateHandler([&](void* origin) { reconfigureAP(); }, false);
}

void APSettingsService::begin() {
  _settingsPersistence.readFromFS();
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
  if (_settings.provisionMode == AP_MODE_ALWAYS ||
      (_settings.provisionMode == AP_MODE_DISCONNECTED && WiFi.status() != WL_CONNECTED)) {
    if (currentWiFiMode == WIFI_OFF || currentWiFiMode == WIFI_STA) {
      startAP();
    }
  } else {
    if (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA) {
      stopAP();
    }
  }
}

void APSettingsService::startAP() {
  Serial.println("Starting software access point");
  WiFi.softAP(_settings.ssid.c_str(), _settings.password.c_str());
  if (!_dnsServer) {
    IPAddress apIp = WiFi.softAPIP();
    Serial.print("Starting captive portal on ");
    Serial.println(apIp);
    _dnsServer = new DNSServer;
    _dnsServer->start(DNS_PORT, "*", apIp);
  }
}

void APSettingsService::stopAP() {
  if (_dnsServer) {
    Serial.println("Stopping captive portal");
    _dnsServer->stop();
    delete _dnsServer;
    _dnsServer = nullptr;
  }
  Serial.println("Stopping software access point");
  WiFi.softAPdisconnect(true);
}

void APSettingsService::handleDNS() {
  if (_dnsServer) {
    _dnsServer->processNextRequest();
  }
}
