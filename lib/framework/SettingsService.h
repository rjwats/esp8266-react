#ifndef SettingsService_h
#define SettingsService_h

#include <functional>

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

typedef std::function<void(void)> SettingsUpdateCallback;
static size_t currentUpdateHandlerId;

typedef struct SettingsUpdateHandlerInfo {
  size_t _id;
  SettingsUpdateCallback _cb;
  bool _allowRemove;
  SettingsUpdateHandlerInfo(SettingsUpdateCallback cb, bool allowRemove) :
      _id(++currentUpdateHandlerId),
      _cb(cb),
      _allowRemove(allowRemove){};
} SettingsUpdateHandlerInfo_t;

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

  size_t addUpdateHandler(SettingsUpdateCallback cb, bool allowRemove = true) {
    if (!cb) {
      return 0;
    }
    SettingsUpdateHandlerInfo_t updateHandler(cb, allowRemove);
    _settingsUpdateHandlers.push_back(updateHandler);
    return updateHandler._id;
  }

  void fetchAsString(String& config) {
    DynamicJsonDocument jsonDocument(MAX_SETTINGS_SIZE);
    fetchAsDocument(jsonDocument);
    serializeJson(jsonDocument, config);
  }

  void updateFromString(String& config) {
    DynamicJsonDocument jsonDocument(MAX_SETTINGS_SIZE);
    deserializeJson(jsonDocument, config);
    updateFromDocument(jsonDocument);
  }

  void fetchAsDocument(JsonDocument& jsonDocument) {
    JsonObject jsonObject = jsonDocument.to<JsonObject>();
    writeToJsonObject(jsonObject);
  }

  void updateFromDocument(JsonDocument& jsonDocument) {
    if (jsonDocument.is<JsonObject>()) {
      JsonObject newConfig = jsonDocument.as<JsonObject>();
      readFromJsonObject(newConfig);
      writeToFS();
      callUpdateHandlers();
    }
  }

  void begin() {
    // read the initial data from the file system
    readFromFS();
  }

 protected:
  char const* _servicePath;
  AsyncJsonWebHandler _updateHandler;
  std::list<SettingsUpdateHandlerInfo_t> _settingsUpdateHandlers;

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
          new AsyncJsonCallbackResponse([this]() { callUpdateHandlers(); }, false, MAX_SETTINGS_SIZE);
      JsonObject jsonObject = response->getRoot();
      writeToJsonObject(jsonObject);
      response->setLength();
      request->send(response);
    } else {
      request->send(400);
    }
  }

  void callUpdateHandlers() {
    // call the classes own config update function
    onConfigUpdated();

    // call all setting update handlers
    for (const SettingsUpdateHandlerInfo_t& handler : _settingsUpdateHandlers) {
      handler._cb();
    }
  }

  // implement to perform action when config has been updated
  virtual void onConfigUpdated() {
  }
};

#endif  // end SettingsService
