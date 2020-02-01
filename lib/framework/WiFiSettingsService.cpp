#include <WiFiSettingsService.h>

WiFiSettingsService::WiFiSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    AdminSettingsService(server, fs, securityManager, WIFI_SETTINGS_SERVICE_PATH, WIFI_SETTINGS_FILE) {
  // We want the device to come up in opmode=0 (WIFI_OFF), when erasing the flash this is not the default.
  // If needed, we save opmode=0 before disabling persistence so the device boots with WiFi disabled in the future.
  if (WiFi.getMode() != WIFI_OFF) {
    WiFi.mode(WIFI_OFF);
  }

  // Disable WiFi config persistance and auto reconnect
  WiFi.persistent(false);
  WiFi.setAutoReconnect(false);
#ifdef ESP32
  // Init the wifi driver on ESP32
  WiFi.mode(WIFI_MODE_MAX);
  WiFi.mode(WIFI_MODE_NULL);
  WiFi.onEvent(
      std::bind(&WiFiSettingsService::onStationModeDisconnected, this, std::placeholders::_1, std::placeholders::_2),
      WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(std::bind(&WiFiSettingsService::onStationModeStop, this, std::placeholders::_1, std::placeholders::_2),
               WiFiEvent_t::SYSTEM_EVENT_STA_STOP);
#elif defined(ESP8266)
  _onStationModeDisconnectedHandler = WiFi.onStationModeDisconnected(
      std::bind(&WiFiSettingsService::onStationModeDisconnected, this, std::placeholders::_1));
#endif
}

WiFiSettingsService::~WiFiSettingsService() {
}

void WiFiSettingsService::begin() {
  SettingsService::begin();
  reconfigureWiFiConnection();
}

void WiFiSettingsService::readFromJsonObject(JsonObject& root) {
  _settings.ssid = root["ssid"] | "";
  _settings.password = root["password"] | "";
  _settings.hostname = root["hostname"] | "";
  _settings.staticIPConfig = root["static_ip_config"] | false;

  // extended settings
  readIP(root, "local_ip", _settings.localIP);
  readIP(root, "gateway_ip", _settings.gatewayIP);
  readIP(root, "subnet_mask", _settings.subnetMask);
  readIP(root, "dns_ip_1", _settings.dnsIP1);
  readIP(root, "dns_ip_2", _settings.dnsIP2);

  // Swap around the dns servers if 2 is populated but 1 is not
  if (_settings.dnsIP1 == INADDR_NONE && _settings.dnsIP2 != INADDR_NONE) {
    _settings.dnsIP1 = _settings.dnsIP2;
    _settings.dnsIP2 = INADDR_NONE;
  }

  // Turning off static ip config if we don't meet the minimum requirements
  // of ipAddress, gateway and subnet. This may change to static ip only
  // as sensible defaults can be assumed for gateway and subnet
  if (_settings.staticIPConfig &&
      (_settings.localIP == INADDR_NONE || _settings.gatewayIP == INADDR_NONE || _settings.subnetMask == INADDR_NONE)) {
    _settings.staticIPConfig = false;
  }
}

void WiFiSettingsService::writeToJsonObject(JsonObject& root) {
  // connection settings
  root["ssid"] = _settings.ssid;
  root["password"] = _settings.password;
  root["hostname"] = _settings.hostname;
  root["static_ip_config"] = _settings.staticIPConfig;

  // extended settings
  writeIP(root, "local_ip", _settings.localIP);
  writeIP(root, "gateway_ip", _settings.gatewayIP);
  writeIP(root, "subnet_mask", _settings.subnetMask);
  writeIP(root, "dns_ip_1", _settings.dnsIP1);
  writeIP(root, "dns_ip_2", _settings.dnsIP2);
}

void WiFiSettingsService::onConfigUpdated() {
  reconfigureWiFiConnection();
}

void WiFiSettingsService::reconfigureWiFiConnection() {
  // reset last connection attempt to force loop to reconnect immediately
  _lastConnectionAttempt = 0;

// disconnect and de-configure wifi
#ifdef ESP32
  if (WiFi.disconnect(true)) {
    _stopping = true;
  }
#elif defined(ESP8266)
  WiFi.disconnect(true);
#endif
}

void WiFiSettingsService::readIP(JsonObject& root, String key, IPAddress& _ip) {
  if (!root[key].is<String>() || !_ip.fromString(root[key].as<String>())) {
    _ip = INADDR_NONE;
  }
}

void WiFiSettingsService::writeIP(JsonObject& root, String key, IPAddress& _ip) {
  if (_ip != INADDR_NONE) {
    root[key] = _ip.toString();
  }
}

void WiFiSettingsService::loop() {
  unsigned long currentMillis = millis();
  if (!_lastConnectionAttempt || (unsigned long)(currentMillis - _lastConnectionAttempt) >= WIFI_RECONNECTION_DELAY) {
    _lastConnectionAttempt = currentMillis;
    manageSTA();
  }
}

void WiFiSettingsService::manageSTA() {
  // Abort if already connected, or if we have no SSID
  if (WiFi.isConnected() || _settings.ssid.length() == 0) {
    return;
  }
  // Connect or reconnect as required
  if ((WiFi.getMode() & WIFI_STA) == 0) {
    Serial.println("Connecting to WiFi.");
    if (_settings.staticIPConfig) {
      // configure for static IP
      WiFi.config(_settings.localIP, _settings.gatewayIP, _settings.subnetMask, _settings.dnsIP1, _settings.dnsIP2);
    } else {
      // configure for DHCP
#ifdef ESP32
      WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
      WiFi.setHostname(_settings.hostname.c_str());
#elif defined(ESP8266)
      WiFi.config(INADDR_ANY, INADDR_ANY, INADDR_ANY);
      WiFi.hostname(_settings.hostname);
#endif
    }
    // attempt to connect to the network
    WiFi.begin(_settings.ssid.c_str(), _settings.password.c_str());
  }
}

#ifdef ESP32
void WiFiSettingsService::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  WiFi.disconnect(true);
}
void WiFiSettingsService::onStationModeStop(WiFiEvent_t event, WiFiEventInfo_t info) {
  if (_stopping) {
    _lastConnectionAttempt = 0;
    _stopping = false;
  }
}
#elif defined(ESP8266)
void WiFiSettingsService::onStationModeDisconnected(const WiFiEventStationModeDisconnected& event) {
  WiFi.disconnect(true);
}
#endif
