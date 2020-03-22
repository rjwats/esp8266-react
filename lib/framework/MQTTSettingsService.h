#ifndef MQTTSettingsService_h
#define MQTTSettingsService_h

#include <SettingsEndpoint.h>
#include <SettingsPersistence.h>
#include <AsyncMqttClient.h>

#define MQTT_RECONNECTION_DELAY 5000

#define MQTT_SETTINGS_FILE "/config/mqttSettings.json"
#define MQTT_SETTINGS_SERVICE_PATH "/rest/mqttSettings"

#define MAX_MQTT_STATUS_SIZE 1024
#define MQTT_STATUS_SERVICE_PATH "/rest/mqttStatus"

#define MQTT_SETTINGS_SERVICE_DEFAULT_ENABLED false
#define MQTT_SETTINGS_SERVICE_DEFAULT_HOST "test.mosquitto.org"
#define MQTT_SETTINGS_SERVICE_DEFAULT_PORT 1883
#define MQTT_SETTINGS_SERVICE_DEFAULT_USERNAME ""
#define MQTT_SETTINGS_SERVICE_DEFAULT_PASSWORD ""
#define MQTT_SETTINGS_SERVICE_DEFAULT_CLIENT_ID generateClientId()
#define MQTT_SETTINGS_SERVICE_DEFAULT_KEEP_ALIVE 16
#define MQTT_SETTINGS_SERVICE_DEFAULT_CLEAN_SESSION true
#define MQTT_SETTINGS_SERVICE_DEFAULT_MAX_TOPIC_LENGTH 128

class MQTTSettings {
 public:
  // host and port - if enabled
  bool enabled;
  String host;
  uint16_t port;

  // username and password
  String username;
  String password;

  // client id settings
  String clientId;

  // connection settings
  uint16_t keepAlive;
  bool cleanSession;
  uint16_t maxTopicLength;
};

static String generateClientId() {
#ifdef ESP32
  return "esp32-" + String((unsigned long)ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
  return "esp8266-" + String(ESP.getChipId(), HEX);
#endif
}

class MQTTSettingsSerializer : public SettingsSerializer<MQTTSettings> {
 public:
  void serialize(MQTTSettings& settings, JsonObject root) {
    root["enabled"] = settings.enabled;
    root["host"] = settings.host;
    root["port"] = settings.port;
    root["username"] = settings.username;
    root["password"] = settings.password;
    root["client_id"] = settings.clientId;
    root["keep_alive"] = settings.keepAlive;
    root["clean_session"] = settings.cleanSession;
    root["max_topic_length"] = settings.maxTopicLength;
  }
};

class MQTTSettingsDeserializer : public SettingsDeserializer<MQTTSettings> {
 public:
  void deserialize(MQTTSettings& settings, JsonObject root) {
    settings.enabled = root["enabled"] | MQTT_SETTINGS_SERVICE_DEFAULT_ENABLED;
    settings.host = root["host"] | MQTT_SETTINGS_SERVICE_DEFAULT_HOST;
    settings.port = root["port"] | MQTT_SETTINGS_SERVICE_DEFAULT_PORT;
    settings.username = root["username"] | MQTT_SETTINGS_SERVICE_DEFAULT_USERNAME;
    settings.password = root["password"] | MQTT_SETTINGS_SERVICE_DEFAULT_PASSWORD;
    settings.clientId = root["client_id"] | MQTT_SETTINGS_SERVICE_DEFAULT_CLIENT_ID;
    settings.keepAlive = root["keep_alive"] | MQTT_SETTINGS_SERVICE_DEFAULT_KEEP_ALIVE;
    settings.cleanSession = root["clean_session"] | MQTT_SETTINGS_SERVICE_DEFAULT_CLEAN_SESSION;
    settings.maxTopicLength = root["max_topic_length"] | MQTT_SETTINGS_SERVICE_DEFAULT_MAX_TOPIC_LENGTH;
  }
};

class MQTTSettingsService : public SettingsService<MQTTSettings> {
 public:
  MQTTSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  ~MQTTSettingsService();

  void begin();
  void loop();
  bool isEnabled();
  bool isConnected();
  const char* getClientId();
  AsyncMqttClientDisconnectReason getDisconnectReason();
  AsyncMqttClient* getMqttClient();

 protected:
  void onConfigUpdated();

 private:
  SettingsEndpoint<MQTTSettings> _settingsEndpoint;
  SettingsPersistence<MQTTSettings> _settingsPersistence;

  AsyncMqttClient _mqttClient;
  bool _reconfigureMqtt;
  unsigned long _disconnectedAt;

  // connection status
  bool _connected;
  AsyncMqttClientDisconnectReason _disconnectReason;

#ifdef ESP32
  void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
  void onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ESP8266)
  WiFiEventHandler _onStationModeDisconnectedHandler;
  WiFiEventHandler _onStationModeGotIPHandler;
  void onStationModeGotIP(const WiFiEventStationModeGotIP& event);
  void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event);
#endif

  void onMqttConnect(bool sessionPresent);
  void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
  void configureMQTT();
};

#endif  // end MQTTSettingsService_h
