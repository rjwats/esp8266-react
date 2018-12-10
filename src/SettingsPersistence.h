#ifndef SettingsPersistence_h
#define SettingsPersistence_h

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
* Mixin for classes which need to save settings to/from a file on the the file system as JSON.
*/
class SettingsPersistence {

protected:

  // will store and retrieve config from the file system
  FS* _fs;

  // the file path our settings will be saved to
  char const* _filePath;

  bool writeToFS() {
    // create and populate a new json object
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    writeToJsonObject(root);

    // serialize it to filesystem
    File configFile = _fs->open(_filePath, "w");

    // failed to open file, return false
    if (!configFile) {
      return false;
    }

    root.printTo(configFile);
    configFile.close();

    return true;
  }

  void readFromFS(){
    File configFile = _fs->open(_filePath, "r");

    // use defaults if no config found
    if (configFile) {
      // Protect against bad data uploaded to file system
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


    // serialization routene, from local config to JsonObject
    virtual void readFromJsonObject(JsonObject& root){}
    virtual void writeToJsonObject(JsonObject& root){}

    // We assume the readFromJsonObject supplies sensible defaults if an empty object
    // is supplied, this virtual function allows that to be changed.
    virtual void applyDefaultConfig(){
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      readFromJsonObject(root);
    }

  public:

    SettingsPersistence(FS* fs, char const* servicePath, char const* filePath):
      _fs(fs), _filePath(filePath) {}

    virtual ~SettingsPersistence() {}

};

#endif // end SettingsPersistence
