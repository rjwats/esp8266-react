#ifndef MQTTStatus_h
#define MQTTStatus_h

#ifdef ESP32
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESPAsyncTCP.h>
#endif

#include <MQTTSettingsService.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>

#define MAX_MQTT_STATUS_SIZE 1024
#define MQTT_STATUS_SERVICE_PATH "/rest/mqttStatus"

class MQTTStatus {
 public:
  MQTTStatus(AsyncWebServer* server, MQTTSettingsService* mqttSettingsService, SecurityManager* securityManager);

 private:
  MQTTSettingsService* _mqttSettingsService;
  bool _connected;
  AsyncMqttClientDisconnectReason _disconnectReason;

  void mqttStatus(AsyncWebServerRequest* request);
  void onMqttConnect(bool sessionPresent);
  void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
};

#endif  // end MQTTStatus_h
