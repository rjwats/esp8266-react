#ifndef SettingsPostEndpoint_h
#define SettingsPostEndpoint_h

#include <functional>

#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

#include <SecurityManager.h>
#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <SettingsDeserializer.h>

#define MAX_CONTENT_LENGTH 1024
#define SETTINGS_ENDPOINT_ORIGIN_ID "endpoint"

template <class T>
class SettingsPostEndpoint {
 public:
  SettingsPostEndpoint(SettingsDeserializer<T>* settingsDeserializer,
                   SettingsService<T>* settingsManager,
                   AsyncWebServer* server,
                   const String& servicePath,
                   SecurityManager* securityManager,
                   AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsManager),
      _updateHandler(
          servicePath,
          securityManager->wrapCallback(
              std::bind(&SettingsPostEndpoint::updateSettings, this, std::placeholders::_1, std::placeholders::_2),
              authenticationPredicate)) {
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_CONTENT_LENGTH);
    server->addHandler(&_updateHandler);
  }

  SettingsPostEndpoint(SettingsDeserializer<T>* settingsDeserializer,
                   SettingsService<T>* settingsManager,
                   AsyncWebServer* server,
                   const String& servicePath) :
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsManager),
      _updateHandler(servicePath,
                     std::bind(&SettingsPostEndpoint::updateSettings, this, std::placeholders::_1, std::placeholders::_2)) {
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_CONTENT_LENGTH);
    server->addHandler(&_updateHandler);
  }

 protected:
  SettingsDeserializer<T>* _settingsDeserializer;
  SettingsService<T>* _settingsService;
  AsyncCallbackJsonWebHandler _updateHandler;

  void updateSettings(AsyncWebServerRequest* request, JsonVariant& json) {
    if (json.is<JsonObject>()) {
      AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_CONTENT_LENGTH);

      // use callback to update the settings once the response is complete
      request->onDisconnect([this]() { _settingsService->callUpdateHandlers(SETTINGS_ENDPOINT_ORIGIN_ID); });

      // update the settings, deferring the call to the update handlers to when the response is complete
      _settingsService->updateWithoutPropagation([&](T& settings) {
        _settingsDeserializer->deserialize(settings, json.as<JsonObject>());
        _settingsSerializer->serialize(settings, response->getRoot().as<JsonObject>());
      });

      // write the response to the client
      response->setLength();
      request->send(response);
    } else {
      request->send(400);
    }
  }
};

#endif  // end SettingsPostEndpoint
