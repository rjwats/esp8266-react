#ifndef HttpEndpoint_h
#define HttpEndpoint_h

#include <functional>

#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

#include <SecurityManager.h>
#include <StatefulService.h>
#include <JsonSerializer.h>
#include <JsonDeserializer.h>

#define MAX_CONTENT_LENGTH 1024
#define HTTP_ENDPOINT_ORIGIN_ID "http"

template <class T>
class HttpGetEndpoint {
 public:
  HttpGetEndpoint(JsonSerializer<T> jsonSerializer,
                  StatefulService<T>* statefulService,
                  AsyncWebServer* server,
                  const String& servicePath,
                  SecurityManager* securityManager,
                  AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _jsonSerializer(jsonSerializer), _statefulService(statefulService) {
    server->on(servicePath.c_str(),
               HTTP_GET,
               securityManager->wrapRequest(std::bind(&HttpGetEndpoint::fetchSettings, this, std::placeholders::_1),
                                            authenticationPredicate));
  }

  HttpGetEndpoint(JsonSerializer<T> jsonSerializer,
                  StatefulService<T>* statefulService,
                  AsyncWebServer* server,
                  const String& servicePath) :
      _jsonSerializer(jsonSerializer), _statefulService(statefulService) {
    server->on(servicePath.c_str(), HTTP_GET, std::bind(&HttpGetEndpoint::fetchSettings, this, std::placeholders::_1));
  }

 protected:
  JsonSerializer<T> _jsonSerializer;
  StatefulService<T>* _statefulService;

  void fetchSettings(AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_CONTENT_LENGTH);
    JsonObject jsonObject = response->getRoot().to<JsonObject>();
    _statefulService->read(jsonObject, _jsonSerializer);

    response->setLength();
    request->send(response);
  }
};

template <class T>
class HttpPostEndpoint {
 public:
  HttpPostEndpoint(JsonSerializer<T> jsonSerializer,
                   JsonDeserializer<T> jsonDeserializer,
                   StatefulService<T>* statefulService,
                   AsyncWebServer* server,
                   const String& servicePath,
                   SecurityManager* securityManager,
                   AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _jsonSerializer(jsonSerializer),
      _jsonDeserializer(jsonDeserializer),
      _statefulService(statefulService),
      _updateHandler(
          servicePath,
          securityManager->wrapCallback(
              std::bind(&HttpPostEndpoint::updateSettings, this, std::placeholders::_1, std::placeholders::_2),
              authenticationPredicate)) {
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_CONTENT_LENGTH);
    server->addHandler(&_updateHandler);
  }

  HttpPostEndpoint(JsonSerializer<T> jsonSerializer,
                   JsonDeserializer<T> jsonDeserializer,
                   StatefulService<T>* statefulService,
                   AsyncWebServer* server,
                   const String& servicePath) :
      _jsonSerializer(jsonSerializer),
      _jsonDeserializer(jsonDeserializer),
      _statefulService(statefulService),
      _updateHandler(servicePath,
                     std::bind(&HttpPostEndpoint::updateSettings, this, std::placeholders::_1, std::placeholders::_2)) {
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_CONTENT_LENGTH);
    server->addHandler(&_updateHandler);
  }

 protected:
  JsonSerializer<T> _jsonSerializer;
  JsonDeserializer<T> _jsonDeserializer;
  StatefulService<T>* _statefulService;
  AsyncCallbackJsonWebHandler _updateHandler;

  void fetchSettings(AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_CONTENT_LENGTH);
    JsonObject jsonObject = response->getRoot().to<JsonObject>();
    _statefulService->read(jsonObject, _jsonSerializer);

    response->setLength();
    request->send(response);
  }

  void updateSettings(AsyncWebServerRequest* request, JsonVariant& json) {
    if (json.is<JsonObject>()) {
      AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_CONTENT_LENGTH);

      // use callback to update the settings once the response is complete
      request->onDisconnect([this]() { _statefulService->callUpdateHandlers(HTTP_ENDPOINT_ORIGIN_ID); });

      // update the settings, deferring the call to the update handlers to when the response is complete
      _statefulService->updateWithoutPropagation([&](T& settings) {
        JsonObject jsonObject = json.as<JsonObject>();
        _jsonDeserializer(jsonObject, settings);
        jsonObject = response->getRoot().to<JsonObject>();
        _jsonSerializer(settings, jsonObject);
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
  HttpEndpoint(JsonSerializer<T> jsonSerializer,
               JsonDeserializer<T> jsonDeserializer,
               StatefulService<T>* statefulService,
               AsyncWebServer* server,
               const String& servicePath,
               SecurityManager* securityManager,
               AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      HttpGetEndpoint<T>(jsonSerializer,
                         statefulService,
                         server,
                         servicePath,
                         securityManager,
                         authenticationPredicate),
      HttpPostEndpoint<T>(jsonSerializer,
                          jsonDeserializer,
                          statefulService,
                          server,
                          servicePath,
                          securityManager,
                          authenticationPredicate) {
  }

  HttpEndpoint(JsonSerializer<T> jsonSerializer,
               JsonDeserializer<T> jsonDeserializer,
               StatefulService<T>* statefulService,
               AsyncWebServer* server,
               const String& servicePath) :
      HttpGetEndpoint<T>(jsonSerializer, statefulService, server, servicePath),
      HttpPostEndpoint<T>(jsonSerializer, jsonDeserializer, statefulService, server, servicePath) {
  }
};

#endif  // end HttpEndpoint
