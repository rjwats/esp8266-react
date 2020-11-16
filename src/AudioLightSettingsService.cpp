#include <AudioLightSettingsService.h>

AudioLightSettingsService::AudioLightSettingsService(AsyncWebServer* server,
                                                     FS* fs,
                                                     SecurityManager* securityManager,
                                                     LedSettingsService* ledSettingsService,
                                                     FrequencySampler* frequencySampler) :
    _httpEndpoint(std::bind(&AudioLightSettingsService::read, this, std::placeholders::_1, std::placeholders::_2),
                  std::bind(&AudioLightSettingsService::update, this, std::placeholders::_1, std::placeholders::_2),
                  this,
                  server,
                  AUDIO_LIGHT_SERVICE_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _audioLightModeTxRx(
        std::bind(&AudioLightSettingsService::read, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&AudioLightSettingsService::update, this, std::placeholders::_1, std::placeholders::_2),
        this,
        server,
        AUDIO_LIGHT_MODE_WS_PATH,
        securityManager,
        AuthenticationPredicates::IS_AUTHENTICATED) {
  server->on(
      AUDIO_LIGHT_SAVE_MODE_PATH,
      HTTP_POST,
      securityManager->wrapRequest(std::bind(&AudioLightSettingsService::saveModeConfig, this, std::placeholders::_1),
                                   AuthenticationPredicates::IS_AUTHENTICATED));
  server->on(
      AUDIO_LIGHT_LOAD_MODE_PATH,
      HTTP_POST,
      securityManager->wrapRequest(std::bind(&AudioLightSettingsService::loadModeConfig, this, std::placeholders::_1),
                                   AuthenticationPredicates::IS_AUTHENTICATED));
  addUpdateHandler([&](const String& originId) { enableMode(); }, false);
  frequencySampler->addUpdateHandler([&](const String& originId) { handleSample(); }, false);
  ledSettingsService->addUpdateHandler([&](const String& originId) { enableMode(); }, false);
  _modes[0] = new ColorMode(server, fs, securityManager, ledSettingsService, frequencySampler);
  _modes[1] = new RainbowMode(server, fs, securityManager, ledSettingsService, frequencySampler);
  _modes[2] = new LightningMode(server, fs, securityManager, ledSettingsService, frequencySampler);
  _modes[3] = new ConfettiMode(server, fs, securityManager, ledSettingsService, frequencySampler);
  _modes[4] = new FireMode(server, fs, securityManager, ledSettingsService, frequencySampler);
  _modes[5] = new OffMode(server, fs, securityManager, ledSettingsService, frequencySampler);
}

void AudioLightSettingsService::begin() {
  // configure current mode
  _state.currentMode = _modes[0];

  // initialize all modes
  for (uint8_t i = 0; i < NUM_MODES; i++) {
    _modes[i]->begin();
  }
}

void AudioLightSettingsService::loop() {
  _state.currentMode->tick();
}

AudioLightMode* AudioLightSettingsService::getMode(const String& modeId) {
  for (uint8_t i = 0; i < NUM_MODES; i++) {
    AudioLightMode* mode = _modes[i];
    if (mode->getId() == modeId) {
      return mode;
    }
  }
  return nullptr;
}

void AudioLightSettingsService::enableMode() {
  _state.currentMode->enable();
}

void AudioLightSettingsService::handleSample() {
  _state.currentMode->sampleComplete();
}

void AudioLightSettingsService::read(AudioLightSettings& settings, JsonObject& root) {
  if (settings.currentMode) {
    root["mode_id"] = settings.currentMode->getId();
    settings.currentMode->readAsJson(root);
  }
}

StateUpdateResult AudioLightSettingsService::update(JsonObject& root, AudioLightSettings& settings) {
  String modeId = root["mode_id"];

  // change mode if required
  if (settings.currentMode->getId() != modeId) {
    AudioLightMode* mode = getMode(modeId);
    if (!mode) {
      return StateUpdateResult::ERROR;
    }
    settings.currentMode = mode;
    return StateUpdateResult::CHANGED;
  }

  // update mode settings
  return settings.currentMode->updateFromJson(root, LOCAL_ORIGIN);
}

void AudioLightSettingsService::saveModeConfig(AsyncWebServerRequest* request) {
  _state.currentMode->writeToFS();
  request->send(200, "text/plain", "Saved");
}

void AudioLightSettingsService::loadModeConfig(AsyncWebServerRequest* request) {
  _state.currentMode->readFromFS();
  _state.currentMode->enable();
  request->send(200, "text/plain", "Loaded");
}
