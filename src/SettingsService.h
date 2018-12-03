#ifndef SettingsService_h
#define SettingsService_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
#endif

#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <AsyncJsonRequestWebHandler.h>
#include <AsyncJsonCallbackResponse.h>

/**
* At the moment, not expecting settings service to have to deal with large JSON
* files this could be made configurable fairly simply, it's exposed on
* AsyncJsonRequestWebHandler with a setter.
*/
#define MAX_SETTINGS_SIZE 1024

/*
* Abstraction of a service which stores it's settings as JSON in SPIFFS.
*/
class SettingsService {

private:

  char const* _filePath;

  AsyncJsonRequestWebHandler _updateHandler;

  bool writeToSPIFFS() {
    // create and populate a new json object
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    writeToJsonObject(root);

    // serialize it to SPIFFS
    File configFile = SPIFFS.open(_filePath, "w");

    // failed to open file, return false
    if (!configFile) {
      return false;
    }

    root.printTo(configFile);
    configFile.close();

    return true;
  }

  void readFromSPIFFS(){
    File configFile = SPIFFS.open(_filePath, "r");

    // use defaults if no config found
    if (configFile) {
      // Protect against bad data uploaded to SPIFFS
      // We never expect the config file to get very large, so cap it.
      size_t size = configFile.size();
      if (size <= MAX_SETTINGS_SIZE) {
        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(configFile);
        if (root.success()) {
          readFromJsonObject(root);
          configFile.close();
          return;
        }
      }
      configFile.close();
    }

    // If we reach here we have not been successful in loading the config,
    // hard-coded emergency defaults are now applied.
    applyDefaultConfig();
  }

  void fetchConfig(AsyncWebServerRequest *request){
    AsyncJsonResponse * response = new AsyncJsonResponse();
    writeToJsonObject(response->getRoot());
    response->setLength();
    request->send(response);
  }

  void updateConfig(AsyncWebServerRequest *request, JsonVariant &json){
    if (json.is<JsonObject>()){
      JsonObject& newConfig = json.as<JsonObject>();
      readFromJsonObject(newConfig);
      writeToSPIFFS();

      // write settings back with a callback to reconfigure the wifi
      AsyncJsonCallbackResponse * response = new AsyncJsonCallbackResponse([this] () {onConfigUpdated();});
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

    // will store and retrieve config from the file system
    FS* _fs;

    // reads the local config from the
    virtual void readFromJsonObject(JsonObject& root){}
    virtual void writeToJsonObject(JsonObject& root){}

    // implement to perform action when config has been updated
    virtual void onConfigUpdated(){}

    // We assume the readFromJsonObject supplies sensible defaults if an empty object
    // is supplied, this virtual function allows that to be changed.
    virtual void applyDefaultConfig(){
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      readFromJsonObject(root);
    }

  public:

    SettingsService(AsyncWebServer* server, FS* fs, char const* servicePath, char const* filePath):
    _filePath(filePath), _server(server), _fs(fs){

      // configure fetch config handler
      _server->on(servicePath, HTTP_GET, std::bind(&SettingsService::fetchConfig, this, std::placeholders::_1));

      // configure update settings handler
      _updateHandler.setUri(servicePath);
      _updateHandler.setMethod(HTTP_POST);
      _updateHandler.setMaxContentLength(MAX_SETTINGS_SIZE);
      _updateHandler.onRequest(std::bind(&SettingsService::updateConfig, this, std::placeholders::_1, std::placeholders::_2));
      _server->addHandler(&_updateHandler);
    }

    virtual ~SettingsService() {}

    virtual void begin() {
      readFromSPIFFS();
    }

};

#endif // end SettingsService
