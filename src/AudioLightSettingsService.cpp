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
                  AudioLightSettings::update,
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
  _modes[0] = new ColorMode(server, fs, securityManager, ledSettingsService, frequencySampler);
  _currentMode = _modes[0];
}

void AudioLightSettingsService::begin() {
  for (uint8_t i = 0; i < NUM_MODES; i++) {
    _modes[i]->begin();
  }
}

void AudioLightSettingsService::loop() {
  _currentMode->tick();
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

void AudioLightSettingsService::sampleComplete() {
  _currentMode->sampleComplete();
}

StateUpdateResult AudioLightSettingsService::update(JsonObject& root, AudioLightSettings& settings) {
  String modeId = root["mode_id"] | AUDIO_LIGHT_DEFAULT_MODE;
  if (settings.modeId == modeId) {
    return StateUpdateResult::UNCHANGED;
  }

  AudioLightMode* mode = getMode(modeId);
  if (!mode) {
    return StateUpdateResult::ERROR;
  }

  settings.modeId = modeId;
  _currentMode = mode;
  _currentMode->enable();
  return StateUpdateResult::CHANGED;
}

void AudioLightSettingsService::saveModeConfig(AsyncWebServerRequest* request) {
  _currentMode->writeToFS();
  request->send(200, "text/plain", "Saved");
}

void AudioLightSettingsService::loadModeConfig(AsyncWebServerRequest* request) {
  _currentMode->readFromFS();
  _currentMode->enable();
  request->send(200, "text/plain", "Loaded");
}
