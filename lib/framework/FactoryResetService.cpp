#include <FactoryResetService.h>

using namespace std::placeholders;

FactoryResetService::FactoryResetService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    fs(fs) {
  server->on(FACTORY_RESET_SERVICE_PATH,
             HTTP_POST,
             securityManager->wrapRequest(std::bind(&FactoryResetService::handle, this, _1),
                                          AuthenticationPredicates::IS_ADMIN));
}

void FactoryResetService::handle(AsyncWebServerRequest* request) {
  request->onDisconnect(std::bind(&FactoryResetService::performFactoryReset, this));
  request->send(200);
}

void FactoryResetService::performFactoryReset() {
  WiFi.disconnect(true);
  Dir configDirectory = fs->openDir("/config");
  while (configDirectory.next()) {
      fs->remove(configDirectory.fileName());
  }
  delay(200);
  ESP.restart();
}
