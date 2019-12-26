#ifndef SettingsService_h
#define SettingsService_h

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
#include <SecurityManager.h>
#include <SettingsPersistence.h>

/*
 * Abstraction of a service which stores it's settings as JSON in a file system.
 */
class SettingsService : public SettingsPersistence {
 public:
  SettingsService(AsyncWebServer* server, FS* fs, char const* servicePath, char const* filePath) :
      SettingsPersistence(fs, filePath),
      _servicePath(servicePath) {
    server->on(_servicePath, HTTP_GET, std::bind(&SettingsService::fetchConfig, this, std::placeholders::_1));
    _updateHandler.setUri(servicePath);
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_SETTINGS_SIZE);
    _updateHandler.onRequest(
        std::bind(&SettingsService::updateConfig, this, std::placeholders::_1, std::placeholders::_2));
    server->addHandler(&_updateHandler);
  }

  virtual ~SettingsService() {
  }

  void begin() {
    // read the initial data from the file system
    readFromFS();
  }

 protected:
  char const* _servicePath;
  AsyncJsonWebHandler _updateHandler;

  virtual void fetchConfig(AsyncWebServerRequest* request) {
    // handle the request
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_SETTINGS_SIZE);
    JsonObject jsonObject = response->getRoot();
    writeToJsonObject(jsonObject);
    response->setLength();
    request->send(response);
  }

  virtual void updateConfig(AsyncWebServerRequest* request, JsonDocument& jsonDocument) {
    // handle the request
    if (jsonDocument.is<JsonObject>()) {
      JsonObject newConfig = jsonDocument.as<JsonObject>();
      readFromJsonObject(newConfig);
      writeToFS();

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

  // implement to perform action when config has been updated
  virtual void onConfigUpdated() {
  }
};

#endif  // end SettingsService
