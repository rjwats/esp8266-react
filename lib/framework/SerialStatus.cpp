#include <SerialStatus.h>

SerialStatus::SerialStatus(AsyncWebServer* server,
                       SerialSettingsService* serialSettingsService,
                       SecurityManager* securityManager) :
    _serialSettingsService(serialSettingsService) {
  server->on(SERIAL_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&SerialStatus::serialStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
}

void SerialStatus::serialStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_SERIAL_STATUS_SIZE);
  JsonObject root = response->getRoot();

  root["enabled"] = _serialSettingsService->isEnabled();
  root["baud"]  = _serialSettingsService->baud();

  response->setLength();
  request->send(response);
}
