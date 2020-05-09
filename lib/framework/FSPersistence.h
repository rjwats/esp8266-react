#ifndef FSPersistence_h
#define FSPersistence_h

#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <SettingsDeserializer.h>
#include <FS.h>

#define MAX_FILE_SIZE 1024

template <class T>
class FSPersistence {
 public:
  FSPersistence(SettingsSerializer<T> settingsSerializer,
                      SettingsDeserializer<T> settingsDeserializer,
                      SettingsService<T>* settingsService,
                      FS* fs,
                      char const* filePath) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsService),
      _fs(fs),
      _filePath(filePath) {
    enableUpdateHandler();
  }

  void readFromFS() {
    File settingsFile = _fs->open(_filePath, "r");

    if (settingsFile) {
      if (settingsFile.size() <= MAX_FILE_SIZE) {
        DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_FILE_SIZE);
        DeserializationError error = deserializeJson(jsonDocument, settingsFile);
        if (error == DeserializationError::Ok && jsonDocument.is<JsonObject>()) {
          updateSettings(jsonDocument.as<JsonObject>());
          settingsFile.close();
          return;
        }
      }
      settingsFile.close();
    }

    // If we reach here we have not been successful in loading the config,
    // hard-coded emergency defaults are now applied.
    applyDefaults();
  }

  bool writeToFS() {
    // create and populate a new json object
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_FILE_SIZE);
    JsonObject jsonObject = jsonDocument.to<JsonObject>();
    _settingsService->read(jsonObject, _settingsSerializer);

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

  void disableUpdateHandler() {
    if (_updateHandlerId) {
      _settingsService->removeUpdateHandler(_updateHandlerId);
      _updateHandlerId = 0;
    }
  }

  void enableUpdateHandler() {
    if (!_updateHandlerId) {
      _updateHandlerId = _settingsService->addUpdateHandler([&](String originId) { writeToFS(); });
    }
  }

 private:
  SettingsSerializer<T> _settingsSerializer;
  SettingsDeserializer<T> _settingsDeserializer;
  SettingsService<T>* _settingsService;
  FS* _fs;
  char const* _filePath;
  update_handler_id_t _updateHandlerId = 0;

  // update the settings, but do not call propogate
  void updateSettings(JsonObject root) {
    _settingsService->updateWithoutPropagation(root, _settingsDeserializer);
  }

 protected:
  // We assume the deserializer supplies sensible defaults if an empty object
  // is supplied, this virtual function allows that to be changed.
  virtual void applyDefaults() {
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_FILE_SIZE);
    updateSettings(jsonDocument.to<JsonObject>());
  }
};

#endif  // end FSPersistence
