#include <MQTTSettingsService.h>

/**
 * Retains a copy of the cstr provided in the pointer provided using dynamic allocation.
 *
 * Frees the pointer before allocation and leaves it as nullptr if cstr == nullptr.
 */
static char* retainCstr(const char* cstr, char** ptr) {
  // free up previously retained value if exists
  free(*ptr);
  *ptr = nullptr;

  // dynamically allocate and copy cstr (if non null)
  if (cstr != nullptr) {
    *ptr = (char*)malloc(strlen(cstr) + 1);
    strcpy(*ptr, cstr);
  }

  // return reference to pointer for convenience
  return *ptr;
}

MQTTSettingsService::MQTTSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _settingsEndpoint(MQTTSettings::serialize,
                      MQTTSettings::deserialize,
                      this,
                      server,
                      MQTT_SETTINGS_SERVICE_PATH,
                      securityManager),
    _settingsPersistence(MQTTSettings::serialize, MQTTSettings::deserialize, this, fs, MQTT_SETTINGS_FILE) {
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
  return _mqttClient.connected();
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
}

void MQTTSettingsService::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.print("Disconnected from MQTT reason: ");
  Serial.println((uint8_t)reason);
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
    _mqttClient.setServer(retainCstr(_settings.host.c_str(), &_retainedHost), _settings.port);
    if (_settings.username.length() > 0) {
      _mqttClient.setCredentials(
          retainCstr(_settings.username.c_str(), &_retainedUsername),
          retainCstr(_settings.password.length() > 0 ? _settings.password.c_str() : nullptr, &_retainedPassword));
    } else {
      _mqttClient.setCredentials(retainCstr(nullptr, &_retainedUsername), retainCstr(nullptr, &_retainedPassword));
    }
    _mqttClient.setClientId(retainCstr(_settings.clientId.c_str(), &_retainedClientId));
    _mqttClient.setKeepAlive(_settings.keepAlive);
    _mqttClient.setCleanSession(_settings.cleanSession);
    _mqttClient.setMaxTopicLength(_settings.maxTopicLength);
    _mqttClient.connect();
  }
}
