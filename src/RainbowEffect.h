#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <Arduino.h>

#include <LightEffect.h>
#include <JsonUtil.h>
#include <StatefulService.h>

#define RAINBOW_DEFAULT_ROTATE_SPEED 32
#define RAINBOW_DEFAULT_HUE_DELTA 250

class RainbowEffectSettings {
 public:
  uint8_t rotateSpeed;
  uint8_t hueDelta;

  static void read(RainbowEffectSettings& settings, JsonObject& root) {
    root["rotate_speed"] = settings.rotateSpeed;
    root["hue_delta"] = settings.hueDelta;
  }

  static StateUpdateResult update(JsonObject& root, RainbowEffectSettings& settings) {
    settings.rotateSpeed = root["rotate_speed"] | RAINBOW_DEFAULT_ROTATE_SPEED;
    settings.hueDelta = root["hue_delta"] | RAINBOW_DEFAULT_HUE_DELTA;
    return StateUpdateResult::CHANGED;
  }
};

class RainbowEffect : public LightEffectService<RainbowEffectSettings> {
 private:
  unsigned long _lastFrameMicros = 0;
  uint8_t _initialhue = 0;
  bool _refresh = true;

 public:
  RainbowEffect(String id,
                CLEDController* ledController,
                FS* fs,
                AsyncWebServer* server,
                SecurityManager* securityManager) :
      LightEffectService(id,
                         ledController,
                         RainbowEffectSettings::read,
                         RainbowEffectSettings::update,
                         fs,
                         server,
                         securityManager){};
  void loop();
  void activate();
};

#endif
