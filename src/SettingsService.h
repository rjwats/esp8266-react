#ifndef SettingsService_h
#define SettingsService_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <SettingsPersistence.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJsonRequestWebHandler.h>
#include <AsyncArduinoJson6.h>

/*
* Abstraction of a service which stores it's settings as JSON in a file system.
*/
class SettingsService : public SettingsPersistence {

private:

  AsyncJsonRequestWebHandler _updateHandler;

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
      writeToFS();

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

    // implement to perform action when config has been updated
    virtual void onConfigUpdated(){}

  public:

    SettingsService(AsyncWebServer* server, FS* fs, char const* servicePath, char const* filePath):
      SettingsPersistence(fs, servicePath, filePath), _server(server) {

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
      readFromFS();
    }

};

#endif // end SettingsService
