#ifndef Service_h
#define Service_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncArduinoJson6.h>
#include <AsyncJsonWebHandler.h>

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

  void fetchConfig(AsyncWebServerRequest *request){
    AsyncJsonResponse * response = new AsyncJsonResponse(MAX_SETTINGS_SIZE);
    JsonObject jsonObject = response->getRoot();    
    writeToJsonObject(jsonObject);
    response->setLength();
    request->send(response);
  }

  void updateConfig(AsyncWebServerRequest *request, JsonDocument &jsonDocument){
    if (jsonDocument.is<JsonObject>()){
      JsonObject newConfig = jsonDocument.as<JsonObject>();
      readFromJsonObject(newConfig);
 
      // write settings back with a callback to reconfigure the wifi
      AsyncJsonCallbackResponse * response = new AsyncJsonCallbackResponse([this] () {onConfigUpdated();}, MAX_SETTINGS_SIZE);
      JsonObject jsonObject = response->getRoot();    
      writeToJsonObject(jsonObject);
      response->setLength();
      request->send(response);
    } else {
      request->send(400);
    }
  }

  protected:

    // will serve setting endpoints from here
    AsyncWebServer* _server;

    // reads the local config from the
    virtual void readFromJsonObject(JsonObject& root){}
    virtual void writeToJsonObject(JsonObject& root){}

    // implement to perform action when config has been updated
    virtual void onConfigUpdated(){}

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
