#include <APSettingsService.h>

APSettingsService::APSettingsService(AsyncWebServer* server, FS* fs) : SettingsService(server, fs, AP_SETTINGS_SERVICE_PATH, AP_SETTINGS_FILE) {
}

APSettingsService::~APSettingsService() {}

void APSettingsService::loop() {
  unsigned long now = millis();
  if (_manageAtMillis <= now){
    WiFiMode_t currentWiFiMode = WiFi.getMode();
    if (_provisionMode == AP_MODE_ALWAYS || (_provisionMode == AP_MODE_DISCONNECTED && WiFi.status() != WL_CONNECTED)) {
      if (currentWiFiMode == WIFI_OFF || currentWiFiMode == WIFI_STA){
        Serial.println("Starting software access point");
        WiFi.softAP(_ssid.c_str(), _password.c_str());
      }
    } else {
      if (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA){
        Serial.println("Stopping software access point");
        WiFi.softAPdisconnect(true);
      }
    }
    _manageAtMillis = now + MANAGE_NETWORK_DELAY;
  }
}

void APSettingsService::readFromJsonObject(JsonObject& root) {
  _provisionMode = root["provision_mode"] | AP_MODE_ALWAYS;
  switch (_provisionMode) {
    case AP_MODE_ALWAYS:
    case AP_MODE_DISCONNECTED:
    case AP_MODE_NEVER:
      break;
    default:
      _provisionMode = AP_MODE_ALWAYS;
  }
  _ssid = root["ssid"] | AP_DEFAULT_SSID;
  _password = root["password"] | AP_DEFAULT_PASSWORD;
}

void APSettingsService::writeToJsonObject(JsonObject& root) {
  root["provision_mode"] = _provisionMode;
  root["ssid"] = _ssid;
  root["password"] = _password;
}

void APSettingsService::onConfigUpdated() {
  _manageAtMillis = 0;
}
