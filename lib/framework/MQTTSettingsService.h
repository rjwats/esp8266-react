#ifndef MQTTSettingsService_h
#define MQTTSettingsService_h

#include <AdminSettingsService.h>
#include <AsyncMqttClient.h>

#define MQTT_SETTINGS_FILE "/config/mqttSettings.json"
#define MQTT_SETTINGS_SERVICE_PATH "/rest/mqttSettings"

#define MAX_MQTT_STATUS_SIZE 1024
#define MQTT_STATUS_SERVICE_PATH "/rest/mqttStatus"

#define MQTT_SETTINGS_SERVICE_DEFAULT_ENABLED true
#define MQTT_SETTINGS_SERVICE_DEFAULT_HOST "test.mosquitto.org"
#define MQTT_SETTINGS_SERVICE_DEFAULT_PORT 1883
#define MQTT_SETTINGS_SERVICE_DEFAULT_SECURE false
#define MQTT_SETTINGS_SERVICE_DEFAULT_USERNAME ""
#define MQTT_SETTINGS_SERVICE_DEFAULT_PASSWORD ""
#define MQTT_SETTINGS_SERVICE_DEFAULT_CLIENT_ID "esp-react"
#define MQTT_SETTINGS_SERVICE_DEFAULT_KEEP_ALIVE 16
#define MQTT_SETTINGS_SERVICE_DEFAULT_CLEAN_SESSION true
#define MQTT_SETTINGS_SERVICE_DEFAULT_MAX_TOPIC_LENGTH 128

class MQTTSettings {
 public:
  // host and port - if enabled
  bool enabled;
  String host;
  uint16_t port;

#if ASYNC_TCP_SSL_ENABLED
  // TODO - fingerprints - if secure
  bool secure;
#endif
  // TODO - LWT

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

class MQTTSettingsService : public AdminSettingsService<MQTTSettings> {
 public:
  MQTTSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  ~MQTTSettingsService();

  void loop();

  AsyncMqttClient* getMqttClient() {
    return &_mqttClient;
  }

 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
  void onConfigUpdated();

 private:
  AsyncMqttClient _mqttClient;
  bool _reconfigureMqtt;

#ifdef ESP32
  void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
  void onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ESP8266)
  WiFiEventHandler _onStationModeDisconnectedHandler;
  WiFiEventHandler _onStationModeGotIPHandler;
  void onStationModeGotIP(const WiFiEventStationModeGotIP& event);
  void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event);
#endif

  void configureMQTT();
};

extern AsyncMqttClient MqttClient;

#endif  // end MQTTSettingsService_h
