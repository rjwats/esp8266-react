#include <LightStateService.h>

LightStateService::LightStateService(AsyncWebServer* server,
                                     FS* fs,
                                     SecurityManager* securityManager,
                                     AsyncMqttClient* mqttClient,
                                     LightMqttSettingsService* lightMqttSettingsService) :
    _server(server),
    _fs(fs),
    _securityManager(securityManager),
    _httpEndpoint(LightState::read,
                  LightState::update,
                  this,
                  server,
                  LIGHT_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(LightState::haRead, LightState::haUpdate, this, mqttClient),
    _webSocket(LightState::read,
               LightState::update,
               this,
               server,
               LIGHT_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttClient(mqttClient),
    _lightMqttSettingsService(lightMqttSettingsService) {
  // set up led controller
  // TODO - make controller types and pin configurable
  _ledController = &FastLED.addLeds<LED_TYPE, LED_DATA_PIN, COLOR_ORDER>(_leds, NUM_LEDS);

  // configure MQTT callback
  _mqttClient->onConnect(std::bind(&LightStateService::registerConfig, this));

  // configure update handler for when the light settings change
  _lightMqttSettingsService->addUpdateHandler([&](const String& originId) { registerConfig(); }, false);

  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

CLEDController* LightStateService::getLedController() {
  return _ledController;
}

void LightStateService::begin() {
  FastLED.setMaxPowerInMilliWatts(10000);
  _state.ledOn = DEFAULT_LED_STATE;
  _state.color = CRGB::White;
  _state.brightness = 100;
  onConfigUpdated();
}

void LightStateService::loop() {
  if (_refresh) {
    // set the brightness
    FastLED.setBrightness(_state.brightness);

    _currentEffect = nullptr;
    for (auto const& effectPtr : _lightEffects) {
      RegisteredLightEffect* effect = effectPtr.get();
      if (effect->getId().equals(_state.effect)) {
        _currentEffect = effect->getEffect();
      }
    }
  }

  // serve current effect or manual, as required
  if (_currentEffect != nullptr) {
    _currentEffect->loop();
  } else if (_refresh) {
    if (_state.ledOn) {
      fill_solid(_leds, NUM_LEDS, _state.color);
      FastLED.show();
    } else {
      _ledController->clearLeds(NUM_LEDS);
    }
  }

  // clear the refresh flag
  _refresh = false;
}

void LightStateService::onConfigUpdated() {
  _refresh = true;
  // digitalWrite(BLINK_LED, _state.ledOn ? LED_ON : LED_OFF);
}

void LightStateService::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String configTopic;
  String subTopic;
  String pubTopic;

  DynamicJsonDocument doc(256);
  _lightMqttSettingsService->read([&](LightMqttSettings& settings) {
    configTopic = settings.mqttPath + "/config";
    subTopic = settings.mqttPath + "/set";
    pubTopic = settings.mqttPath + "/state";
    doc["~"] = settings.mqttPath;
    doc["name"] = settings.name;
    doc["unique_id"] = settings.uniqueId;
  });
  doc["cmd_t"] = "~/set";
  doc["stat_t"] = "~/state";
  doc["schema"] = "json";
  doc["brightness"] = true;
  doc["rgb"] = true;
  doc["effect"] = true;

  JsonArray effectList = doc.createNestedArray("effect_list");
  effectList.add("Color");
  for (auto const& effectPtr : _lightEffects) {
    RegisteredLightEffect* effect = effectPtr.get();
    effectList.add(effect->getId());
  }

  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

  _mqttPubSub.configureTopics(pubTopic, subTopic);
}
