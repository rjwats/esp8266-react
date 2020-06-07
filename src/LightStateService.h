#ifndef LightStateService_h
#define LightStateService_h

#include <LightMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>
#include <FastLED.h>
#include <JsonUtil.h>
#include <LightEffect.h>
#include <FS.h>
#include <list>
#include <memory>

// fast led settings
#define LED_DATA_PIN 14  // was 21 for esp32
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LEDS 96
#define NUM_MODES 7

#define DEFAULT_EFFECT "Color"
#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"

#define LIGHT_SETTINGS_ENDPOINT_PATH "/rest/lightState"
#define LIGHT_SETTINGS_SOCKET_PATH "/ws/lightState"

class LightState {
 public:
  bool ledOn;
  uint8_t brightness;
  CRGB color;
  LightEffect* effect = nullptr;
};

class LightStateService : public StatefulService<LightState> {
 public:
  LightStateService(AsyncWebServer* server,
                    FS* fs,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    LightMqttSettingsService* lightMqttSettingsService);
  void begin();
  void loop();

  // temp - LED controller should be suppliex externally
  CLEDController* getLedController();

  template <class E>
  void addEffect(LightEffectService<E>* service, JsonStateReader<E> stateReader, JsonStateUpdater<E> stateUpdater) {
    _lightEffects.push_back(std::make_shared<RegisteredLightEffectService<E>>(
        stateReader, stateUpdater, service, _fs, _server, _securityManager));
  }

 private:
  AsyncWebServer* _server;
  FS* _fs;
  SecurityManager* _securityManager;

  HttpEndpoint<LightState> _httpEndpoint;
  MqttPubSub<LightState> _mqttPubSub;
  WebSocketTxRx<LightState> _webSocket;
  AsyncMqttClient* _mqttClient;
  LightMqttSettingsService* _lightMqttSettingsService;

  std::list<std::shared_ptr<RegisteredLightEffect>> _lightEffects;

  bool _refresh;
  CRGB _leds[NUM_LEDS];
  CLEDController* _ledController;

  void registerConfig();
  void onConfigUpdated();

  LightEffect* getEffect(String effectId) {
    for (auto const& effectPtr : _lightEffects) {
      RegisteredLightEffect* effect = effectPtr.get();
      if (effect->getId().equals(effectId)) {
        return effect->getEffect();
      }
    }
    return nullptr;
  }

  void readForService(LightState& lightState, JsonObject& root) {
    root["led_on"] = lightState.ledOn;
    root["color"] = colorToHexString(lightState.color);
    root["brightness"] = lightState.brightness;
    root["effect"] = lightState.effect == nullptr ? DEFAULT_EFFECT : lightState.effect->getId();
  }

  // TODO - validating update?
  StateUpdateResult updateFromService(JsonObject& root, LightState& lightState) {
    lightState.ledOn = root["led_on"] | DEFAULT_LED_STATE;
    String color = root["color"];
    lightState.color = hexStringToColor(color, CRGB::White);
    lightState.brightness = root["brightness"] | 255;
    lightState.effect = getEffect(root["effect"]);
    return StateUpdateResult::CHANGED;
  }

  void readForHa(LightState& lightState, JsonObject& root) {
    root["state"] = lightState.ledOn ? ON_STATE : OFF_STATE;
    colorToRGBJson(lightState.color, root);
    root["brightness"] = lightState.brightness;
    root["effect"] = lightState.effect == nullptr ? DEFAULT_EFFECT : lightState.effect->getId();
  }

  // TODO - validating update?
  StateUpdateResult updateFromHa(JsonObject& root, LightState& lightState) {
    lightState.ledOn = root["state"].as<String>().equals(ON_STATE);
    if (root.containsKey("color")) {
      lightState.color = rgbJsonToColor(root["color"].as<JsonObject>());
    }
    if (root.containsKey("brightness")) {
      lightState.brightness = root["brightness"];
    }
    if (root.containsKey("effect")) {
      lightState.effect = getEffect(root["effect"]);
    }
    return StateUpdateResult::CHANGED;
  }
};

#endif
