#include <MQTTStatus.h>

MQTTStatus::MQTTStatus(AsyncWebServer* server,
                       MQTTSettingsService* mqttSettingsService,
                       SecurityManager* securityManager) :
    _mqttSettingsService(mqttSettingsService) {
  server->on(MQTT_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&MQTTStatus::mqttStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
  _mqttSettingsService->getMqttClient()->onConnect(std::bind(&MQTTStatus::onMqttConnect, this, std::placeholders::_1));
  _mqttSettingsService->getMqttClient()->onDisconnect(
      std::bind(&MQTTStatus::onMqttDisconnect, this, std::placeholders::_1));
}

void MQTTStatus::onMqttConnect(bool sessionPresent) {
  Serial.print("Connected to MQTT, ");
  Serial.print(sessionPresent ? "with" : "without");
  Serial.println(" persistent session");
  _connected = true;
}

void MQTTStatus::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.print("Disconnected from MQTT reason: ");
  Serial.println((uint8_t)reason);
  _connected = false;
  _disconnectReason = reason;
}

void MQTTStatus::mqttStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_MQTT_STATUS_SIZE);
  JsonObject root = response->getRoot();

  root["connected"] = _connected;
  root["client_id"] = _mqttSettingsService->getMqttClient()->getClientId();
  root["disconnect_reason"] = (uint8_t)_disconnectReason;

  response->setLength();
  request->send(response);
}
