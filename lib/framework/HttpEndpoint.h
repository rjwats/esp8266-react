#ifndef HttpEndpoint_h
#define HttpEndpoint_h

#include <functional>

#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

#include <SecurityManager.h>
#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <SettingsDeserializer.h>

#define MAX_CONTENT_LENGTH 1024
#define HTTP_ENDPOINT_ORIGIN_ID "http"

template <class T>
class HttpGetEndpoint {
 public:
  HttpGetEndpoint(SettingsSerializer<T> settingsSerializer,
                  SettingsService<T>* settingsService,
                  AsyncWebServer* server,
                  const String& servicePath,
                  SecurityManager* securityManager,
                  AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _settingsSerializer(settingsSerializer), _settingsService(settingsService) {
    server->on(servicePath.c_str(),
               HTTP_GET,
               securityManager->wrapRequest(std::bind(&HttpGetEndpoint::fetchSettings, this, std::placeholders::_1),
                                            authenticationPredicate));
  }

  HttpGetEndpoint(SettingsSerializer<T> settingsSerializer,
                  SettingsService<T>* settingsService,
                  AsyncWebServer* server,
                  const String& servicePath) :
      _settingsSerializer(settingsSerializer), _settingsService(settingsService) {
    server->on(servicePath.c_str(), HTTP_GET, std::bind(&HttpGetEndpoint::fetchSettings, this, std::placeholders::_1));
  }

 protected:
  SettingsSerializer<T> _settingsSerializer;
  SettingsService<T>* _settingsService;

  void fetchSettings(AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_CONTENT_LENGTH);
    JsonObject jsonObject = response->getRoot().to<JsonObject>();
    _settingsService->read(jsonObject, _settingsSerializer);

    response->setLength();
    request->send(response);
  }
};

template <class T>
class HttpPostEndpoint {
 public:
  HttpPostEndpoint(SettingsSerializer<T> settingsSerializer,
                   SettingsDeserializer<T> settingsDeserializer,
                   SettingsService<T>* settingsService,
                   AsyncWebServer* server,
                   const String& servicePath,
                   SecurityManager* securityManager,
                   AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsService),
      _updateHandler(
          servicePath,
          securityManager->wrapCallback(
              std::bind(&HttpPostEndpoint::updateSettings, this, std::placeholders::_1, std::placeholders::_2),
              authenticationPredicate)) {
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_CONTENT_LENGTH);
    server->addHandler(&_updateHandler);
  }

  HttpPostEndpoint(SettingsSerializer<T> settingsSerializer,
                   SettingsDeserializer<T> settingsDeserializer,
                   SettingsService<T>* settingsService,
                   AsyncWebServer* server,
                   const String& servicePath) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsService),
      _updateHandler(servicePath,
                     std::bind(&HttpPostEndpoint::updateSettings, this, std::placeholders::_1, std::placeholders::_2)) {
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_CONTENT_LENGTH);
    server->addHandler(&_updateHandler);
  }

 protected:
  SettingsSerializer<T> _settingsSerializer;
  SettingsDeserializer<T> _settingsDeserializer;
  SettingsService<T>* _settingsService;
  AsyncCallbackJsonWebHandler _updateHandler;

  void fetchSettings(AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_CONTENT_LENGTH);
    JsonObject jsonObject = response->getRoot().to<JsonObject>();
    _settingsService->read(jsonObject, _settingsSerializer);

    response->setLength();
    request->send(response);
  }

  void updateSettings(AsyncWebServerRequest* request, JsonVariant& json) {
    if (json.is<JsonObject>()) {
      AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_CONTENT_LENGTH);

      // use callback to update the settings once the response is complete
      request->onDisconnect([this]() { _settingsService->callUpdateHandlers(HTTP_ENDPOINT_ORIGIN_ID); });

      // update the settings, deferring the call to the update handlers to when the response is complete
      _settingsService->updateWithoutPropagation([&](T& settings) {
        JsonObject jsonObject = json.as<JsonObject>();
        _settingsDeserializer(jsonObject, settings);
        jsonObject = response->getRoot().to<JsonObject>();
        _settingsSerializer(settings, jsonObject);
      });

      // write the response to the client
      response->setLength();
      request->send(response);
    } else {
      request->send(400);
    }
  }
};

template <class T>
class HttpEndpoint : public HttpGetEndpoint<T>, public HttpPostEndpoint<T> {
 public:
  HttpEndpoint(SettingsSerializer<T> settingsSerializer,
               SettingsDeserializer<T> settingsDeserializer,
               SettingsService<T>* settingsService,
               AsyncWebServer* server,
               const String& servicePath,
               SecurityManager* securityManager,
               AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      HttpGetEndpoint<T>(settingsSerializer,
                         settingsService,
                         server,
                         servicePath,
                         securityManager,
                         authenticationPredicate),
      HttpPostEndpoint<T>(settingsSerializer,
                          settingsDeserializer,
                          settingsService,
                          server,
                          servicePath,
                          securityManager,
                          authenticationPredicate) {
  }

  HttpEndpoint(SettingsSerializer<T> settingsSerializer,
               SettingsDeserializer<T> settingsDeserializer,
               SettingsService<T>* settingsService,
               AsyncWebServer* server,
               const String& servicePath) :
      HttpGetEndpoint<T>(settingsSerializer, settingsService, server, servicePath),
      HttpPostEndpoint<T>(settingsSerializer, settingsDeserializer, settingsService, server, servicePath) {
  }
};

#endif  // end HttpEndpoint
