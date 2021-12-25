#include <Ser2netStatus.h>

Ser2netStatus::Ser2netStatus(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(SER2NET_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&Ser2netStatus::ser2netStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
}

void Ser2netStatus::ser2netStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_SER2NET_STATUS_SIZE);
  JsonObject root = response->getRoot();

  // grab the current instant in unix seconds
  time_t now = time(nullptr);

  // only provide enabled/disabled status for now
  root["enabled"] = 1;

  // device uptime in seconds
  root["uptime"] = millis() / 1000;

  response->setLength();
  request->send(response);
}
