#ifndef AudioLightMode_h
#define AudioLightMode_h

#include <LedSettingsService.h>
#include <LedSettingsService.h>
#include <LedSettingsService.h>

template <class T>
class AudioLightMode : public StatefulService<T> {
 protected:
  LedSettingsService* _ledSettingsService;
  HttpEndpoint<LedSettings> _httpEndpoint;
  FSPersistence<LedSettings> _fsPersistence;

 public:
  // TODO - replace the two strings with a single id - string template or concatication?
  AudioLightMode(AsyncWebServer* server,
                 FS* fs,
                 SecurityManager* securityManager,
                 JsonStateReader<T> stateReader,
                 JsonStateUpdater<T> stateUpdater,
                 const String& settingsServicePath,
                 const String& setttingsFile) :
      _httpEndpoint(stateReader, stateUpdater, this, server, settingsServicePath, securityManager),
      _fsPersistence(stateReader, stateUpdater, this, fs, setttingsFile) {
  }

  /*
   * Get the code for the mode as a string
   */
  virtual String getId() = 0;

  /*
   * Allow the mode to animate the LEDs - called by the main loop.
   */
  virtual void tick() = 0;

  /**
   * Called when the mode is enabled.
   */
  virtual void enable() = 0;

};

#endif  // end AudioLightMode_h
