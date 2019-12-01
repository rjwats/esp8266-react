#include <RestartService.h>

RestartService::RestartService(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(RESTART_SERVICE_PATH, HTTP_POST, securityManager->wrapRequest(
    std::bind(&RestartService::restart, this, std::placeholders::_1), AuthenticationPredicates::IS_ADMIN
  ));
}

void RestartService::restart(AsyncWebServerRequest* request) {
  request->onDisconnect([]() {
#if defined(ESP8266)
    ESP.reset();
#elif defined(ESP_PLATFORM)
    ESP.restart();
#endif
  });
  request->send(200);
}
