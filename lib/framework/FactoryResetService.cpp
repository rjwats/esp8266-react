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
  request->onDisconnect(std::bind(&FactoryResetService::factoryReset, this));
  request->send(200);
}

void FactoryResetService::factoryReset() {
  WiFi.disconnect(true);
#ifdef ESP32
  fs->rmdir("/config");
#elif defined(ESP8266)
  Dir configDirectory = fs->openDir("/config");
  while (configDirectory.next()) {
      fs->remove(configDirectory.fileName());
  }
#endif  
  delay(200);
#ifdef ESP32
    ESP.restart();
#elif defined(ESP8266)
    ESP.reset();
#endif
}
