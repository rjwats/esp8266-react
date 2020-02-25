#include <MQTTSettingsService.h>

MQTTSettingsService::MQTTSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    AdminSettingsService(server, fs, securityManager, MQTT_SETTINGS_SERVICE_PATH, MQTT_SETTINGS_FILE) {
#ifdef ESP32
  WiFi.onEvent(
      std::bind(&MQTTSettingsService::onStationModeDisconnected, this, std::placeholders::_1, std::placeholders::_2),
      WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(std::bind(&MQTTSettingsService::onStationModeGotIP, this, std::placeholders::_1, std::placeholders::_2),
               WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
#elif defined(ESP8266)
  _onStationModeDisconnectedHandler = WiFi.onStationModeDisconnected(
      std::bind(&MQTTSettingsService::onStationModeDisconnected, this, std::placeholders::_1));
  _onStationModeGotIPHandler =
      WiFi.onStationModeGotIP(std::bind(&MQTTSettingsService::onStationModeGotIP, this, std::placeholders::_1));
#endif
}

MQTTSettingsService::~MQTTSettingsService() {
}

void MQTTSettingsService::loop() {
  // TODO - reconnection handler
  if (_reconfigureMqtt) {
    _reconfigureMqtt = false;
    configureMQTT();
  }
}

void MQTTSettingsService::readFromJsonObject(JsonObject& root) {
  _settings.enabled = root["enabled"] | MQTT_SETTINGS_SERVICE_DEFAULT_ENABLED;
  _settings.host = root["host"] | MQTT_SETTINGS_SERVICE_DEFAULT_HOST;

  // YUCK! Is this an ArduinoJSON bug? The pipe syntax default is always being appled for integers!
  _settings.port = MQTT_SETTINGS_SERVICE_DEFAULT_PORT;
  if (root.containsKey("port")) {
    _settings.port = root["port"];
  }

#if ASYNC_TCP_SSL_ENABLED
  _settings.secure = root["secure"] | MQTT_SETTINGS_SERVICE_DEFAULT_SECURE;
#endif
  _settings.username = root["username"] | MQTT_SETTINGS_SERVICE_DEFAULT_USERNAME;
  _settings.password = root["password"] | MQTT_SETTINGS_SERVICE_DEFAULT_PASSWORD;
  _settings.clientId = root["client_id"] | MQTT_SETTINGS_SERVICE_DEFAULT_CLIENT_ID;
  _settings.keepAlive = root["keep_alive"] | MQTT_SETTINGS_SERVICE_DEFAULT_KEEP_ALIVE;
  _settings.cleanSession = root["clean_session"] | MQTT_SETTINGS_SERVICE_DEFAULT_CLEAN_SESSION;
  _settings.maxTopicLength = root["max_topic_length"] | MQTT_SETTINGS_SERVICE_DEFAULT_MAX_TOPIC_LENGTH;
}

void MQTTSettingsService::writeToJsonObject(JsonObject& root) {
  root["enabled"] = _settings.enabled;
  root["host"] = _settings.host;
  root["port"] = _settings.port;
#if ASYNC_TCP_SSL_ENABLED
  root["secure"] = _settings.secure;
#endif
  root["username"] = _settings.username;
  root["password"] = _settings.password;
  root["client_id"] = _settings.clientId;
  root["keep_alive"] = _settings.keepAlive;
  root["clean_session"] = _settings.cleanSession;
  root["max_topic_length"] = _settings.maxTopicLength;
}

void MQTTSettingsService::onConfigUpdated() {
  _reconfigureMqtt = true;
}

#ifdef ESP32
void MQTTSettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  _reconfigureMqtt = true;
}

void MQTTSettingsService::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connection dropped, stopping MQTT client.");
  _reconfigureMqtt = false;
  _mqttClient.disconnect();
}
#elif defined(ESP8266)
void MQTTSettingsService::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  _reconfigureMqtt = true;
}

void MQTTSettingsService::onStationModeDisconnected(const WiFiEventStationModeDisconnected& event) {
  Serial.println("WiFi connection dropped, stopping MQTT client.");
  _reconfigureMqtt = false;
  _mqttClient.disconnect();
}
#endif

void MQTTSettingsService::configureMQTT() {
  Serial.println("Configuring MQTT...");
  _mqttClient.disconnect();
  if (_settings.enabled) {
    _mqttClient.setServer(_settings.host.c_str(), _settings.port);
#if ASYNC_TCP_SSL_ENABLED
    _mqttClient.setSecure(_settings.secure);
#endif
    if (_settings.username.length() > 0) {
      const char* username = _settings.username.c_str();
      const char* password = _settings.password.length() > 0 ? _settings.password.c_str() : nullptr;
      _mqttClient.setCredentials(username, password);
    } else {
      _mqttClient.setCredentials(nullptr, nullptr);
    }
    _mqttClient.setClientId(_settings.clientId.c_str());
    _mqttClient.setKeepAlive(_settings.keepAlive);
    _mqttClient.setCleanSession(_settings.cleanSession);
    _mqttClient.setMaxTopicLength(_settings.maxTopicLength);
    _mqttClient.connect();
  }
}
