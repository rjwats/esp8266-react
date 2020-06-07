#ifndef LightEffect_h
#define LightEffect_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <StatefulService.h>
#include <FSPersistence.h>
#include <HttpEndpoint.h>

#define LIGHT_EFFECT_FS_DIRECTORY "/effect/"
#define LIGHT_EFFECT_REST_PREFIX "/rest/effect/"

class LightEffect {
 protected:
  CLEDController* _ledController;

 public:
  LightEffect(CLEDController* ledController) : _ledController(ledController) {
  }

  /**
   * Get the ID of the effect - max ~25 chars
   */
  virtual String getId() = 0;

  /**
   * Called when the effect is activated.
   */
  virtual void activate() = 0;

  /*
   * Called by the main loop on the active effect.
   */
  virtual void loop() = 0;
};

template <class T>
class LightEffectService : public LightEffect, public StatefulService<T> {
 public:
  LightEffectService(CLEDController* ledController) : LightEffect(ledController), StatefulService<T>() {
  }
  /**
   * Read the current configuration to a JsonObject
   */
  virtual void readSettings(T& settings, JsonObject& root) = 0;

  /**
   * Update the current configuration from a JsonObject
   */
  virtual StateUpdateResult updateSettings(JsonObject& root, T& settings) = 0;
};

class RegisteredLightEffect {
 public:
  RegisteredLightEffect(LightEffect* lightEffect) : _lightEffect(lightEffect) {
  }

  String getId() {
    return _lightEffect->getId();
  }

  LightEffect* getEffect() {
    return _lightEffect;
  }

  virtual void begin() = 0;

 protected:
  LightEffect* _lightEffect;
};

template <class T>
class RegisteredLightEffectService : public RegisteredLightEffect {
 public:
  RegisteredLightEffectService(JsonStateReader<T> stateReader,
                               JsonStateUpdater<T> stateUpdater,
                               LightEffectService<T>* service,
                               FS* fs,
                               AsyncWebServer* server,
                               SecurityManager* securityManager) :
      RegisteredLightEffect(service),
      _fsPersistence(stateReader,
                     stateUpdater,
                     service,
                     fs,
                     String(LIGHT_EFFECT_FS_DIRECTORY + service->getId()).c_str()),
      _httpEndpoint(stateReader,
                    stateUpdater,
                    service,
                    server,
                    String(LIGHT_EFFECT_REST_PREFIX + service->getId()).c_str(),
                    securityManager,
                    AuthenticationPredicates::IS_AUTHENTICATED) {
  }

  void begin() {
    _fsPersistence.readFromFS();
  }

 protected:
  FSPersistence<T> _fsPersistence;
  HttpEndpoint<T> _httpEndpoint;
};

#endif
