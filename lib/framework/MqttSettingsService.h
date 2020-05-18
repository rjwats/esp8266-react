#ifndef MqttSettingsService_h
#define MqttSettingsService_h

#include <StatefulService.h>
#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <AsyncMqttClient.h>

#define MQTT_RECONNECTION_DELAY 5000

#define MQTT_SETTINGS_FILE "/config/mqttSettings.json"
#define MQTT_SETTINGS_SERVICE_PATH "/rest/mqttSettings"

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_ENABLED
  #define MQTT_SETTINGS_SERVICE_DEFAULT_ENABLED false
#endif

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_HOST
  #define MQTT_SETTINGS_SERVICE_DEFAULT_HOST "test.mosquitto.org"
#endif

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_PORT
  #define MQTT_SETTINGS_SERVICE_DEFAULT_PORT 1883
#endif

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_USERNAME
  #define MQTT_SETTINGS_SERVICE_DEFAULT_USERNAME ""
#endif

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_PASSWORD
  #define MQTT_SETTINGS_SERVICE_DEFAULT_PASSWORD ""
#endif

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_CLIENT_ID
  #define MQTT_SETTINGS_SERVICE_DEFAULT_CLIENT_ID generateClientId()
#endif

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_KEEP_ALIVE
  #define MQTT_SETTINGS_SERVICE_DEFAULT_KEEP_ALIVE 16
#endif

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_CLEAN_SESSION
  #define MQTT_SETTINGS_SERVICE_DEFAULT_CLEAN_SESSION true
#endif

#ifndef MQTT_SETTINGS_SERVICE_DEFAULT_MAX_TOPIC_LENGTH
  #define MQTT_SETTINGS_SERVICE_DEFAULT_MAX_TOPIC_LENGTH 128
#endif

static String generateClientId() {
#ifdef ESP32
  return "esp32-" + String((unsigned long)ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
  return "esp8266-" + String(ESP.getChipId(), HEX);
#endif
}

class MqttSettings {
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

  static void serialize(MqttSettings& settings, JsonObject& root) {
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

  static void deserialize(JsonObject& root, MqttSettings& settings) {
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

class MqttSettingsService : public StatefulService<MqttSettings> {
 public:
  MqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  ~MqttSettingsService();

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
  HttpEndpoint<MqttSettings> _httpEndpoint;
  FSPersistence<MqttSettings> _fsPersistence;

  // Pointers to hold retained copies of the mqtt client connection strings.
  // Required as AsyncMqttClient holds refrences to the supplied connection strings.
  char* _retainedHost = nullptr;
  char* _retainedClientId = nullptr;
  char* _retainedUsername = nullptr;
  char* _retainedPassword = nullptr;

  AsyncMqttClient _mqttClient;
  bool _reconfigureMqtt;
  unsigned long _disconnectedAt;

  // connection status
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
  void configureMqtt();
};

#endif  // end MqttSettingsService_h
