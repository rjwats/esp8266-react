#include <ResetService.h>

ResetService::ResetService(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(RESET_SERVICE_PATH, HTTP_POST, securityManager->wrapRequest(
    std::bind(&ResetService::reset, this, std::placeholders::_1), AuthenticationPredicates::IS_ADMIN
  ));
}

void ResetService::reset(AsyncWebServerRequest* request) {
  request->onDisconnect([]() {
#if defined(ESP8266)
    ESP.reset();
#elif defined(ESP_PLATFORM)
    ESP.restart();
#endif
  });
  request->send(200);
}
