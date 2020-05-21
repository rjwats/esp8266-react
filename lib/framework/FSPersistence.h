#ifndef FSPersistence_h
#define FSPersistence_h

#include <StatefulService.h>
#include <JsonSerializer.h>
#include <JsonDeserializer.h>
#include <FS.h>

#define MAX_FILE_SIZE 1024

template <class T>
class FSPersistence {
 public:
  FSPersistence(JsonSerializer<T> jsonSerializer,
                JsonDeserializer<T> jsonDeserializer,
                StatefulService<T>* statefulService,
                FS* fs,
                char const* filePath) :
      _jsonSerializer(jsonSerializer),
      _jsonDeserializer(jsonDeserializer),
      _statefulService(statefulService),
      _fs(fs),
      _filePath(filePath),
      _updateHandlerId(0) {
    enableUpdateHandler();
  }

  void readFromFS() {
    File settingsFile = _fs->open(_filePath, "r");

    if (settingsFile) {
      if (settingsFile.size() <= MAX_FILE_SIZE) {
        DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_FILE_SIZE);
        DeserializationError error = deserializeJson(jsonDocument, settingsFile);
        if (error == DeserializationError::Ok && jsonDocument.is<JsonObject>()) {
          JsonObject jsonObject = jsonDocument.as<JsonObject>();
          _statefulService->updateWithoutPropagation(jsonObject, _jsonDeserializer);
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
    _statefulService->read(jsonObject, _jsonSerializer);

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
      _statefulService->removeUpdateHandler(_updateHandlerId);
      _updateHandlerId = 0;
    }
  }

  void enableUpdateHandler() {
    if (!_updateHandlerId) {
      _updateHandlerId = _statefulService->addUpdateHandler([&](const String& originId) { writeToFS(); });
    }
  }

 private:
  JsonSerializer<T> _jsonSerializer;
  JsonDeserializer<T> _jsonDeserializer;
  StatefulService<T>* _statefulService;
  FS* _fs;
  char const* _filePath;
  update_handler_id_t _updateHandlerId;

 protected:
  // We assume the deserializer supplies sensible defaults if an empty object
  // is supplied, this virtual function allows that to be changed.
  virtual void applyDefaults() {
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_FILE_SIZE);
    JsonObject jsonObject = jsonDocument.as<JsonObject>();
    _statefulService->updateWithoutPropagation(jsonObject, _jsonDeserializer);
  }
};

#endif  // end FSPersistence
