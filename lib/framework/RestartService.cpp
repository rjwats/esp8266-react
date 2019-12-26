#include <RestartService.h>

RestartService::RestartService(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(RESTART_SERVICE_PATH,
             HTTP_POST,
             securityManager->wrapRequest(std::bind(&RestartService::restart, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_ADMIN));
}

void RestartService::restart(AsyncWebServerRequest* request) {
  request->onDisconnect([]() {
#ifdef ESP32
    ESP.restart();
#elif defined(ESP8266)
    ESP.reset();
#endif
  });
  request->send(200);
}
