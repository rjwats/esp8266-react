#ifndef LightEffect_h
#define LightEffect_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLed.h>
#include <StatefulService.h>
#include <FSPersistence.h>
#include <HttpEndpoint.h>

#define LIGHT_EFFECT_FS_DIRECTORY "/effect/"
#define LIGHT_EFFECT_REST_PREFIX "/rest/effect/"

class LightEffect {
 protected:
  String _id;
  CLEDController* _ledController;

 public:
  LightEffect(String id, CLEDController* ledController) : _id(id), _ledController(ledController) {
  }

  String getId() {
    return _id;
  }

  /*
   * Allow the effect to control the LEDs.
   *
   * Called by the main loop on the active effect.
   */
  virtual void loop() = 0;

  /**
   * Called when the effect is activated.
   */
  virtual void activate() = 0;
};

template <class T>
class LightEffectService : public LightEffect, public StatefulService<T> {
 public:
  LightEffectService(String id,
                     CLEDController* ledController,
                     JsonStateReader<T> stateReader,
                     JsonStateUpdater<T> stateUpdater,
                     FS* fs,
                     AsyncWebServer* server,
                     SecurityManager* securityManager) :
      LightEffect(id, ledController),
      StatefulService<T>(),
      _fsPersistence(stateReader, stateUpdater, this, fs, String(LIGHT_EFFECT_FS_DIRECTORY + id).c_str()),
      _httpEndpoint(stateReader,
                    stateUpdater,
                    this,
                    server,
                    String(LIGHT_EFFECT_REST_PREFIX + id).c_str(),
                    securityManager,
                    AuthenticationPredicates::IS_AUTHENTICATED) {
  }

 protected:
  FSPersistence<T> _fsPersistence;
  HttpEndpoint<T> _httpEndpoint;
};

#endif
