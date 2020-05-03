#ifndef SettingsGetEndpoint_h
#define SettingsGetEndpoint_h

#include <functional>

#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

#include <SecurityManager.h>
#include <SettingsService.h>
#include <SettingsSerializer.h>

#define MAX_CONTENT_LENGTH 1024

template <class T>
class SettingsGetEndpoint {
 public:
  SettingsGetEndpoint(SettingsSerializer<T>* settingsSerializer,
                   SettingsService<T>* settingsManager,
                   AsyncWebServer* server,
                   const String& servicePath,
                   SecurityManager* securityManager,
                   AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _settingsSerializer(settingsSerializer),
      _settingsService(settingsManager) {
    server->on(servicePath.c_str(),
               HTTP_GET,
               securityManager->wrapRequest(std::bind(&SettingsGetEndpoint::fetchSettings, this, std::placeholders::_1),
                                            authenticationPredicate));
  }

  SettingsGetEndpoint(SettingsSerializer<T>* settingsSerializer,
                   SettingsService<T>* settingsManager,
                   AsyncWebServer* server,
                   const String& servicePath) :
      _settingsSerializer(settingsSerializer),
      _settingsService(settingsManager) {
    server->on(servicePath.c_str(), HTTP_GET, std::bind(&SettingsGetEndpoint::fetchSettings, this, std::placeholders::_1));
  }

 protected:
  SettingsSerializer<T>* _settingsSerializer;
  SettingsService<T>* _settingsService;

  void fetchSettings(AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_CONTENT_LENGTH);
    _settingsService->read(
        [&](T& settings) { _settingsSerializer->serialize(settings, response->getRoot().to<JsonObject>()); });
    response->setLength();
    request->send(response);
  }

};

#endif  // end SettingsEndpoint
