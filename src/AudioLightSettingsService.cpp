#include <AudioLightSettingsService.h>

void junkBodyHandler(AsyncWebServerRequest* request,
                     String filename,
                     size_t index,
                     uint8_t* data,
                     size_t len,
                     bool final) {
}

AudioLightSettingsService::AudioLightSettingsService(AsyncWebServer* server,
                                                     FS* fs,
                                                     SecurityManager* securityManager,
                                                     LedSettingsService* ledSettingsService,
                                                     FrequencySampler* frequencySampler) :
    _httpEndpoint(AudioLightSettings::read,
                  std::bind(&AudioLightSettingsService::update, this, std::placeholders::_1, std::placeholders::_2),
                  this,
                  server,
                  AUDIO_LIGHT_SERVICE_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED) {
  server->on(AUDIO_LIGHT_SAVE_MODE_PATH,
             HTTP_POST,
             std::bind(&AudioLightSettingsService::saveModeConfig, this, std::placeholders::_1),
             junkBodyHandler);
  server->on(AUDIO_LIGHT_LOAD_MODE_PATH,
             HTTP_POST,
             std::bind(&AudioLightSettingsService::loadModeConfig, this, std::placeholders::_1),
             junkBodyHandler);
  frequencySampler->addUpdateHandler([&](const String& originId) { sampleComplete(); }, false);
  addUpdateHandler([&](const String& originId) { modeChanged(); }, false);
  _modes[0] = new ColorMode(server, fs, securityManager, ledSettingsService, frequencySampler);
  _modes[1] = new RainbowMode(server, fs, securityManager, ledSettingsService, frequencySampler);
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

void AudioLightSettingsService::modeChanged() {
  _state.currentMode->enable();
}

void AudioLightSettingsService::sampleComplete() {
  _state.currentMode->sampleComplete();
}

StateUpdateResult AudioLightSettingsService::update(JsonObject& root, AudioLightSettings& settings) {
  String modeId = root["mode_id"] | AUDIO_LIGHT_DEFAULT_MODE;
  if (settings.currentMode->getId() == modeId) {
    return StateUpdateResult::UNCHANGED;
  }
  AudioLightMode* mode = getMode(modeId);
  if (!mode) {
    return StateUpdateResult::ERROR;
  }
  settings.currentMode = mode;
  return StateUpdateResult::CHANGED;
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
