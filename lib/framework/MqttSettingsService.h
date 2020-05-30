#ifndef MqttSettingsService_h
#define MqttSettingsService_h

#include <StatefulService.h>
#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <AsyncMqttClient.h>
#include <ESPUtils.h>

#define MQTT_RECONNECTION_DELAY 5000

#define MQTT_SETTINGS_FILE "/config/mqttSettings.json"
#define MQTT_SETTINGS_SERVICE_PATH "/rest/mqttSettings"

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

  static void read(MqttSettings& settings, JsonObject& root) {
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

  static StateUpdateResult update(JsonObject& root, MqttSettings& settings) {
    settings.enabled = root["enabled"] | FACTORY_MQTT_ENABLED;
    settings.host = root["host"] | FACTORY_MQTT_HOST;
    settings.port = root["port"] | FACTORY_MQTT_PORT;
    settings.username = root["username"] | FactoryValue::format(FACTORY_MQTT_USERNAME);
    settings.password = root["password"] | FACTORY_MQTT_PASSWORD;
    settings.clientId = root["client_id"] | FactoryValue::format(FACTORY_MQTT_CLIENT_ID);
    settings.keepAlive = root["keep_alive"] | FACTORY_MQTT_KEEP_ALIVE;
    settings.cleanSession = root["clean_session"] | FACTORY_MQTT_CLEAN_SESSION;
    settings.maxTopicLength = root["max_topic_length"] | FACTORY_MQTT_MAX_TOPIC_LENGTH;
    return StateUpdateResult::CHANGED;
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
  // This is required as AsyncMqttClient holds refrences to the supplied connection strings.
  char* _retainedHost;
  char* _retainedClientId;
  char* _retainedUsername;
  char* _retainedPassword;

  // variable to help manage connection
  bool _reconfigureMqtt;
  unsigned long _disconnectedAt;

  // connection status
  AsyncMqttClientDisconnectReason _disconnectReason;

  // the MQTT client instance
  AsyncMqttClient _mqttClient;

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
