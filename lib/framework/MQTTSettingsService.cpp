#include <MQTTSettingsService.h>

static MQTTSettingsSerializer SERIALIZER;
static MQTTSettingsDeserializer DESERIALIZER;

MQTTSettingsService::MQTTSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _settingsEndpoint(&SERIALIZER, &DESERIALIZER, this, server, MQTT_SETTINGS_SERVICE_PATH, securityManager),
    _settingsPersistence(&SERIALIZER, &DESERIALIZER, this, fs, MQTT_SETTINGS_FILE) {
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
  _mqttClient.onConnect(std::bind(&MQTTSettingsService::onMqttConnect, this, std::placeholders::_1));
  _mqttClient.onDisconnect(std::bind(&MQTTSettingsService::onMqttDisconnect, this, std::placeholders::_1));
  addUpdateHandler([&](String originId) { onConfigUpdated(); }, false);
}

MQTTSettingsService::~MQTTSettingsService() {
}

void MQTTSettingsService::begin() {
  _settingsPersistence.readFromFS();
}

void MQTTSettingsService::loop() {
  if (_reconfigureMqtt || (_disconnectedAt && (unsigned long)(millis() - _disconnectedAt) >= MQTT_RECONNECTION_DELAY)) {
    // reconfigure MQTT client
    configureMQTT();

    // clear the reconnection flags
    _reconfigureMqtt = false;
    _disconnectedAt = 0;
  }
}

bool MQTTSettingsService::isEnabled() {
  return _settings.enabled;
}

bool MQTTSettingsService::isConnected() {
  return _connected;
}

const char* MQTTSettingsService::getClientId() {
  return _mqttClient.getClientId();
}

AsyncMqttClientDisconnectReason MQTTSettingsService::getDisconnectReason() {
  return _disconnectReason;
}

AsyncMqttClient* MQTTSettingsService::getMqttClient() {
  return &_mqttClient;
}

void MQTTSettingsService::onMqttConnect(bool sessionPresent) {
  Serial.print("Connected to MQTT, ");
  Serial.print(sessionPresent ? "with" : "without");
  Serial.println(" persistent session");
  _connected = true;
}

void MQTTSettingsService::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.print("Disconnected from MQTT reason: ");
  Serial.println((uint8_t)reason);
  _connected = false;
  _disconnectReason = reason;
  _disconnectedAt = millis();
}

void MQTTSettingsService::onConfigUpdated() {
  _reconfigureMqtt = true;
  _disconnectedAt = 0;
}

#ifdef ESP32
void MQTTSettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  if (_settings.enabled) {
    Serial.println("WiFi connection dropped, starting MQTT client.");
    onConfigUpdated();
  }
}

void MQTTSettingsService::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  if (_settings.enabled) {
    Serial.println("WiFi connection dropped, stopping MQTT client.");
    onConfigUpdated();
  }
}
#elif defined(ESP8266)
void MQTTSettingsService::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  if (_settings.enabled) {
    Serial.println("WiFi connection dropped, starting MQTT client.");
    onConfigUpdated();
  }
}

void MQTTSettingsService::onStationModeDisconnected(const WiFiEventStationModeDisconnected& event) {
  if (_settings.enabled) {
    Serial.println("WiFi connection dropped, stopping MQTT client.");
    onConfigUpdated();
  }
}
#endif

void MQTTSettingsService::configureMQTT() {
  // disconnect if currently connected
  _mqttClient.disconnect();

  // only connect if WiFi is connected and MQTT is enabled
  if (_settings.enabled && WiFi.isConnected()) {
    Serial.println("Connecting to MQTT...");
    _mqttClient.setServer(_settings.host.c_str(), _settings.port);
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
