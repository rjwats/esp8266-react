#ifndef SettingsService_h
#define SettingsService_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <SecurityManager.h>
#include <SettingsPersistence.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJsonWebHandler.h>
#include <AsyncArduinoJson6.h>


/*
* Abstraction of a service which stores it's settings as JSON in a file system.
*/
class SettingsService : public SettingsPersistence {

  public:

    SettingsService(AsyncWebServer* server, FS* fs, char const* servicePath, char const* filePath):
      SettingsPersistence(fs, filePath), _server(server) {

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

protected:
  // will serve setting endpoints from here
  AsyncWebServer* _server;

  AsyncJsonWebHandler _updateHandler;

  virtual void fetchConfig(AsyncWebServerRequest *request) {
    // handle the request
    AsyncJsonResponse * response = new AsyncJsonResponse(MAX_SETTINGS_SIZE);
    JsonObject jsonObject = response->getRoot();  
    writeToJsonObject(jsonObject);
    response->setLength();
    request->send(response);
  }

  virtual void updateConfig(AsyncWebServerRequest *request, JsonDocument &jsonDocument) {
    // handle the request
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

  // implement to perform action when config has been updated
  virtual void onConfigUpdated(){}

};

class AdminSettingsService : public SettingsService {
  public:  
    AdminSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager, char const* servicePath, char const* filePath):
      SettingsService(server, fs, servicePath, filePath), _securityManager(securityManager) {
    }

  protected:
    // will validate the requests with the security manager
    SecurityManager* _securityManager;

    void fetchConfig(AsyncWebServerRequest *request) {
      // verify the request against the predicate
      Authentication authentication = _securityManager->authenticateRequest(request);
      if (!getAuthenticationPredicate()(authentication)) {
        request->send(401);
        return;
      }
      // delegate to underlying implemetation
      SettingsService::fetchConfig(request);
    }

    void updateConfig(AsyncWebServerRequest *request, JsonDocument &jsonDocument) {
      // verify the request against the predicate
      Authentication authentication = _securityManager->authenticateRequest(request);
      if (!getAuthenticationPredicate()(authentication)) {
        request->send(401);
        return;
      }
      // delegate to underlying implemetation
      SettingsService::updateConfig(request, jsonDocument);
    }

    // override to override the default authentication predicate, IS_ADMIN
    AuthenticationPredicate getAuthenticationPredicate() {
      return AuthenticationPredicates::IS_ADMIN;
    }

};

#endif // end SettingsService
