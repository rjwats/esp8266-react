#ifndef SettingsEndpoint_h
#define SettingsEndpoint_h

#include <functional>

#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

#include <SecurityManager.h>
#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <SettingsDeserializer.h>

#define MAX_SETTINGS_SIZE 1024

template <class T>
class SettingsEndpoint {
 public:
  SettingsEndpoint(SettingsSerializer<T>* settingsSerializer,
                   SettingsDeserializer<T>* settingsDeserializer,
                   SettingsService<T>* settingsManager,
                   AsyncWebServer* server,
                   const String& servicePath,
                   SecurityManager* securityManager,
                   AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsManager),
      _updateHandler(
          servicePath,
          securityManager->wrapCallback(
              std::bind(&SettingsEndpoint::updateSettings, this, std::placeholders::_1, std::placeholders::_2),
              authenticationPredicate)) {
    server->on(servicePath.c_str(),
               HTTP_GET,
               securityManager->wrapRequest(std::bind(&SettingsEndpoint::fetchSettings, this, std::placeholders::_1),
                                            authenticationPredicate));
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_SETTINGS_SIZE);
    server->addHandler(&_updateHandler);
  }

  SettingsEndpoint(SettingsSerializer<T>* settingsSerializer,
                   SettingsDeserializer<T>* settingsDeserializer,
                   SettingsService<T>* settingsManager,
                   AsyncWebServer* server,
                   const String& servicePath) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsManager),
      _updateHandler(servicePath,
                     std::bind(&SettingsEndpoint::updateSettings, this, std::placeholders::_1, std::placeholders::_2)) {
    server->on(servicePath.c_str(), HTTP_GET, std::bind(&SettingsEndpoint::fetchSettings, this, std::placeholders::_1));
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_SETTINGS_SIZE);
    server->addHandler(&_updateHandler);
  }

 protected:
  SettingsSerializer<T>* _settingsSerializer;
  SettingsDeserializer<T>* _settingsDeserializer;
  SettingsService<T>* _settingsService;
  AsyncCallbackJsonWebHandler _updateHandler;

  void fetchSettings(AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_SETTINGS_SIZE);
    _settingsService->read(
        [&](T& settings) { _settingsSerializer->serialize(settings, response->getRoot().as<JsonObject>()); });
    response->setLength();
    request->send(response);
  }

  void updateSettings(AsyncWebServerRequest* request, JsonVariant& json) {
    if (json.is<JsonObject>()) {
      AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_SETTINGS_SIZE);

      // use callback to update the settings once the response is complete
      request->onDisconnect([this]() { _settingsService->callUpdateHandlers(); });

      // update the settings, deferring the call to the update handlers to when the response is complete
      _settingsService->withSettings(
          [&](T& settings) {
            _settingsDeserializer->deserialize(settings, json.as<JsonObject>());
            _settingsSerializer->serialize(settings, response->getRoot().as<JsonObject>());
          },
          false);

      // write the response to the client
      response->setLength();
      request->send(response);
    } else {
      request->send(400);
    }
  }
};

#endif  // end SettingsEndpoint
