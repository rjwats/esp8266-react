#include <WiFiSettingsService.h>

WiFiSettingsService::WiFiSettingsService(AsyncWebServer* server, FS* fs) : SettingsService(server, fs, WIFI_SETTINGS_SERVICE_PATH, WIFI_SETTINGS_FILE) {
}

WiFiSettingsService::~WiFiSettingsService() {}

void WiFiSettingsService::begin() {
  SettingsService::begin();
  reconfigureWiFiConnection();
}

void WiFiSettingsService::readFromJsonObject(JsonObject& root){
    _ssid = root["ssid"] | "";
    _password = root["password"] | "";
    _hostname = root["hostname"] | "";
    _staticIPConfig = root["static_ip_config"] | false;

    // extended settings
    readIP(root, "local_ip", _localIP);
    readIP(root, "gateway_ip", _gatewayIP);
    readIP(root, "subnet_mask", _subnetMask);
    readIP(root, "dns_ip_1", _dnsIP1);
    readIP(root, "dns_ip_2", _dnsIP2);

    // Swap around the dns servers if 2 is populated but 1 is not
    if (_dnsIP1 == 0U && _dnsIP2 != 0U){
      _dnsIP1 = _dnsIP2;
      _dnsIP2 = 0U;
    }

    // Turning off static ip config if we don't meet the minimum requirements
    // of ipAddress, gateway and subnet. This may change to static ip only
    // as sensible defaults can be assumed for gateway and subnet
    if (_staticIPConfig && (_localIP == 0U || _gatewayIP == 0U || _subnetMask == 0U)){
      _staticIPConfig = false;
    }
}

void WiFiSettingsService::writeToJsonObject(JsonObject& root){
      // connection settings
      root["ssid"] = _ssid;
      root["password"] = _password;
      root["hostname"] = _hostname;
      root["static_ip_config"] = _staticIPConfig;

      // extended settings
      writeIP(root, "local_ip", _localIP);
      writeIP(root, "gateway_ip", _gatewayIP);
      writeIP(root, "subnet_mask", _subnetMask);
      writeIP(root, "dns_ip_1", _dnsIP1);
      writeIP(root, "dns_ip_2", _dnsIP2);
}

void WiFiSettingsService::onConfigUpdated() {
  reconfigureWiFiConnection();
}

void WiFiSettingsService::reconfigureWiFiConnection() {
    Serial.println("Reconfiguring WiFi...");

    // disconnect and de-configure wifi and software access point
    WiFi.disconnect(true);

    // configure static ip config for station mode (if set)
    if (_staticIPConfig) {
      WiFi.config(_localIP, _gatewayIP,  _subnetMask, _dnsIP1, _dnsIP2);
    }

    // connect to the network
    WiFi.hostname(_hostname);
    WiFi.begin(_ssid.c_str(), _password.c_str());
}

void WiFiSettingsService::readIP(JsonObject& root, String key, IPAddress& _ip){
  if (!root[key] || !_ip.fromString(root[key].as<String>())){
    _ip = 0U;
  }
}

void WiFiSettingsService::writeIP(JsonObject& root, String key, IPAddress& _ip){
  if (_ip != 0U){
    root[key] = _ip.toString();
  }
}
