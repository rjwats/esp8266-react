#include <MQTTStatus.h>

MQTTStatus::MQTTStatus(AsyncWebServer* server,
                       MQTTSettingsService* mqttSettingsService,
                       SecurityManager* securityManager) :
    _mqttSettingsService(mqttSettingsService) {
  server->on(MQTT_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&MQTTStatus::mqttStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
}

void MQTTStatus::mqttStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_MQTT_STATUS_SIZE);
  JsonObject root = response->getRoot();

  root["enabled"] = _mqttSettingsService->isEnabled();
  root["connected"] = _mqttSettingsService->isConnected();
  root["client_id"] = _mqttSettingsService->getClientId();
  root["disconnect_reason"] = (uint8_t)_mqttSettingsService->getDisconnectReason();

  response->setLength();
  request->send(response);
}
