#ifndef SettingsPersistence_h
#define SettingsPersistence_h

#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <SettingsDeserializer.h>
#include <FS.h>

#define MAX_SETTINGS_SIZE 1024

/**
 * SettingsPersistance takes care of loading and saving settings when they change.
 *
 * SettingsPersistence automatically registers it's writeToFS function as an update handler with the settings manager
 * when constructed, saving any updates to the file system.
 */
template <class T>
class SettingsPersistence {
 public:
  SettingsPersistence(SettingsSerializer<T>* settingsSerializer,
                      SettingsDeserializer<T>* settingsDeserializer,
                      SettingsService<T>* settingsManager,
                      FS* fs,
                      char const* filePath) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsManager(settingsManager),
      _fs(fs),
      _filePath(filePath) {
    _settingsManager->addUpdateHandler([&](void* origin) { writeToFS(); }, false);
  }

  void readFromFS() {
    File settingsFile = _fs->open(_filePath, "r");

    if (settingsFile) {
      if (settingsFile.size() <= MAX_SETTINGS_SIZE) {
        DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_SETTINGS_SIZE);
        DeserializationError error = deserializeJson(jsonDocument, settingsFile);
        if (error == DeserializationError::Ok && jsonDocument.is<JsonObject>()) {
          readSettings(jsonDocument.as<JsonObject>());
          settingsFile.close();
          return;
        }
      }
      settingsFile.close();
    }

    // If we reach here we have not been successful in loading the config,
    // hard-coded emergency defaults are now applied.
    readDefaults();
  }

  bool writeToFS() {
    // create and populate a new json object
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_SETTINGS_SIZE);
    _settingsManager->update(
        [&](T& settings) { _settingsSerializer->serialize(settings, jsonDocument.to<JsonObject>()); }, false);

    // serialize it to filesystem
    File settingsFile = _fs->open(_filePath, "w");

    // failed to open file, return false
    if (!settingsFile) {
      return false;
    }

    // serialize the data to the file
    serializeJson(jsonDocument, settingsFile);
    settingsFile.close();
    return true;
  }

 private:
  SettingsSerializer<T>* _settingsSerializer;
  SettingsDeserializer<T>* _settingsDeserializer;
  SettingsService<T>* _settingsManager;
  FS* _fs;
  char const* _filePath;

  // read the settings, but do not call propogate
  void readSettings(JsonObject root) {
    _settingsManager->update([&](T& settings) { _settingsDeserializer->deserialize(settings, root); }, false);
  }

 protected:
  // We assume the readFromJsonObject supplies sensible defaults if an empty object
  // is supplied, this virtual function allows that to be changed.
  virtual void readDefaults() {
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_SETTINGS_SIZE);
    readSettings(jsonDocument.to<JsonObject>());
  }
};

#endif  // end SettingsPersistence
