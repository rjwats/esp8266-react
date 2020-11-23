#ifndef AudioLightMode_h
#define AudioLightMode_h

#include <LedSettingsService.h>
#include <PaletteSettingsService.h>
#include <FrequencySampler.h>

#define AUDIO_LIGHT_MODE_FILE_PATH_PREFIX "/modes/"
#define AUDIO_LIGHT_MODE_FILE_PATH_SUFFIX ".json"
#define AUDIO_LIGHT_MODE_SERVICE_PATH_PREFIX "/rest/modes/"

class AudioLightMode {
 public:
  virtual const String& getId() = 0;
  virtual void begin() = 0;
  virtual void readFromFS() = 0;
  virtual void writeToFS() = 0;
  virtual void tick() = 0;
  virtual void enable() = 0;
  virtual void sampleComplete(){};
  virtual void readAsJson(JsonObject& root) = 0;
  virtual StateUpdateResult updateFromJson(JsonObject& root, const String& originId) = 0;
};

template <class T>
class AudioLightModeImpl : public StatefulService<T>, public AudioLightMode {
 protected:
  String _id;
  String _filePath;
  String _servicePath;
  JsonStateReader<T> _stateReader;
  JsonStateUpdater<T> _stateUpdater;
  LedSettingsService* _ledSettingsService;
  PaletteSettingsService* _paletteSettingsService;
  FrequencySampler* _frequencySampler;
  HttpEndpoint<T> _httpEndpoint;
  FSPersistence<T> _fsPersistence;

 public:
  AudioLightModeImpl(AsyncWebServer* server,
                     FS* fs,
                     SecurityManager* securityManager,
                     LedSettingsService* ledSettingsService,
                     PaletteSettingsService* paletteSettingsService,
                     FrequencySampler* frequencySampler,
                     JsonStateReader<T> stateReader,
                     JsonStateUpdater<T> stateUpdater,
                     const String& id) :
      _id(id),
      _filePath(AUDIO_LIGHT_MODE_SERVICE_PATH_PREFIX + id),
      _servicePath(AUDIO_LIGHT_MODE_FILE_PATH_PREFIX + id + AUDIO_LIGHT_MODE_FILE_PATH_SUFFIX),
      _stateReader(stateReader),
      _stateUpdater(stateUpdater),
      _ledSettingsService(ledSettingsService),
      _paletteSettingsService(paletteSettingsService),
      _frequencySampler(frequencySampler),
      _httpEndpoint(stateReader, stateUpdater, this, server, _filePath, securityManager),
      _fsPersistence(stateReader, stateUpdater, this, fs, _servicePath.c_str()) {
  }

  /*
   * Get the code for the mode as a string
   */
  const String& getId() {
    return _id;
  }

  /*
   * Read the config from the file system and disable the update handler
   */
  void begin() {
    _fsPersistence.disableUpdateHandler();
    _fsPersistence.readFromFS();
  }

  /*
   * Load the mode config from the file system
   */
  void readFromFS() {
    _fsPersistence.readFromFS();
  }

  /*
   * Save the mode config to the file system
   */
  void writeToFS() {
    _fsPersistence.writeToFS();
  }

  /*
   * Read the mode settings as json
   */
  void readAsJson(JsonObject& root) {
    StatefulService<T>::read(root, _stateReader);
  }

  /*
   * Update the mode settings from json
   */
  StateUpdateResult updateFromJson(JsonObject& root, const String& originId) {
    return StatefulService<T>::update(root, _stateUpdater, originId);
  }
};

#endif  // end AudioLightMode_h
