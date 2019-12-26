#ifndef Service_h
#define Service_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncJsonCallbackResponse.h>
#include <AsyncJsonWebHandler.h>
#include <ESPAsyncWebServer.h>

/**
 * At the moment, not expecting services to have to deal with large JSON
 * files this could be made configurable fairly simply, it's exposed on
 * AsyncJsonWebHandler with a setter.
 */
#define MAX_SETTINGS_SIZE 1024

/*
 * Abstraction of a service which reads and writes data from an endpoint.
 *
 * Not currently used, but indended for use by features which do not
 * require setting persistance.
 */
class SimpleService {
 private:
  AsyncJsonWebHandler _updateHandler;

  void fetchConfig(AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_SETTINGS_SIZE);
    JsonObject jsonObject = response->getRoot();
    writeToJsonObject(jsonObject);
    response->setLength();
    request->send(response);
  }

  void updateConfig(AsyncWebServerRequest* request, JsonDocument& jsonDocument) {
    if (jsonDocument.is<JsonObject>()) {
      JsonObject newConfig = jsonDocument.as<JsonObject>();
      readFromJsonObject(newConfig);

      // write settings back with a callback to reconfigure the wifi
      AsyncJsonCallbackResponse* response =
          new AsyncJsonCallbackResponse([this]() { onConfigUpdated(); }, false, MAX_SETTINGS_SIZE);
      JsonObject jsonObject = response->getRoot();
      writeToJsonObject(jsonObject);
      response->setLength();
      request->send(response);
    } else {
      request->send(400);
    }
  }

 protected:
  // reads the local config from the
  virtual void readFromJsonObject(JsonObject& root) {
  }
  virtual void writeToJsonObject(JsonObject& root) {
  }

  // implement to perform action when config has been updated
  virtual void onConfigUpdated() {
  }

 public:
  SimpleService(AsyncWebServer* server, char const* servicePath) {
    server->on(servicePath, HTTP_GET, std::bind(&SimpleService::fetchConfig, this, std::placeholders::_1));

    _updateHandler.setUri(servicePath);
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_SETTINGS_SIZE);
    _updateHandler.onRequest(
        std::bind(&SimpleService::updateConfig, this, std::placeholders::_1, std::placeholders::_2));
    server->addHandler(&_updateHandler);
  }

  virtual ~SimpleService() {
  }
};

#endif  // end SimpleService
