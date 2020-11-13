#ifndef AudioLightMode_h
#define AudioLightMode_h

#include <LedSettingsService.h>
#include <LedSettingsService.h>
#include <LedSettingsService.h>
#include <FrequencySampler.h>

#define AUDIO_LIGHT_MODE_FILE_PATH_PREFIX "/modes/"
#define AUDIO_LIGHT_MODE_FILE_PATH_SUFFIX ".json"
#define AUDIO_LIGHT_MODE_SERVICE_PATH_PREFIX "/rest/modes/"

template <class T>
class AudioLightMode : public StatefulService<T> {
 protected:
  String _id;
  LedSettingsService* _ledSettingsService;
  FrequencySampler* _frequencySampler;
  HttpEndpoint<T> _httpEndpoint;
  FSPersistence<T> _fsPersistence;

 public:
  AudioLightMode(AsyncWebServer* server,
                 FS* fs,
                 SecurityManager* securityManager,
                 LedSettingsService* ledSettingsService,
                 FrequencySampler* frequencySampler,
                 JsonStateReader<T> stateReader,
                 JsonStateUpdater<T> stateUpdater,
                 const String& id) :
      _id(id),
      _ledSettingsService(ledSettingsService),
      _frequencySampler(frequencySampler),
      _httpEndpoint(stateReader, stateUpdater, this, server, AUDIO_LIGHT_MODE_FILE_PATH_PREFIX + id, securityManager),
      _fsPersistence(stateReader,
                     stateUpdater,
                     this,
                     fs,
                     String(AUDIO_LIGHT_MODE_FILE_PATH_PREFIX + id + AUDIO_LIGHT_MODE_FILE_PATH_SUFFIX).c_str()) {
  }

  /*
   * Get the code for the mode as a string
   */
  const String& getId() {
    return _id;
  }

  /*
   * Allow the mode to animate the LEDs - called by the main loop.
   */
  virtual void tick() = 0;

  /**
   * Called when the mode is enabled.
   */
  virtual void enable() = 0;

  /**
   * Called when new frequency data is sampled so the mode can update as required.
   *
   * Will only be called for the active mode.
   */
  virtual void sampleComplete(){};
};

#endif  // end AudioLightMode_h
