#ifndef SimpleService_h
#define SimpleService_h

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <AsyncJsonRequestWebHandler.h>

/**
* At the moment, not expecting services to have to deal with large JSON
* files this could be made configurable fairly simply, it's exposed on
* AsyncJsonRequestWebHandler with a setter.
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

  AsyncJsonRequestWebHandler _updateHandler;

  void fetchConfig(AsyncWebServerRequest *request){
    AsyncJsonResponse * response = new AsyncJsonResponse();
    writeToJsonObject(response->getRoot());
    response->setLength();
    request->send(response);
  }

  void updateConfig(AsyncWebServerRequest *request, JsonVariant &json){
    if (json.is<JsonObject>()){
      JsonObject& newConfig = json.as<JsonObject>();
      readFromJsonObject(newConfig, "service");

      // write settings back with a callback to reconfigure the wifi
      AsyncJsonResponse * response = new AsyncJsonResponse();
      writeToJsonObject(response->getRoot());
      response->setLength();
      request->send(response);
    } else{
      request->send(400);
    }
  }

  protected:

    // will serve setting endpoints from here
    AsyncWebServer* _server;

    // reads the local config from the
    virtual void readFromJsonObject(JsonObject& root, String originId){}
    virtual void writeToJsonObject(JsonObject& root) {}

  public:

    SimpleService(AsyncWebServer* server, char const* servicePath):
    _server(server) {

      // configure fetch config handler
      _server->on(servicePath, HTTP_GET, std::bind(&SimpleService::fetchConfig, this, std::placeholders::_1));

      // configure update settings handler
      _updateHandler.setUri(servicePath);
      _updateHandler.setMethod(HTTP_POST);
      _updateHandler.setMaxContentLength(MAX_SETTINGS_SIZE);
      _updateHandler.onRequest(std::bind(&SimpleService::updateConfig, this, std::placeholders::_1, std::placeholders::_2));
      _server->addHandler(&_updateHandler);
    }

    virtual ~SimpleService() {}

};

#endif // end SimpleService
