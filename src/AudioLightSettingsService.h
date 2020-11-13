#ifndef AudioLightSettingsService_h
#define AudioLightSettingsService_h

#include <ColorMode.h>
#include <StatefulService.h>
#include <HttpEndpoint.h>

#define NUM_MODES 1

#define AUDIO_LIGHT_SERVICE_PATH "/rest/mode"
#define AUDIO_LIGHT_SAVE_MODE_PATH "/rest/mode/save"
#define AUDIO_LIGHT_LOAD_MODE_PATH "/rest/mode/load"

#define AUDIO_LIGHT_DEFAULT_MODE "color"

class AudioLightSettings {
 public:
  String modeId;

  static void read(AudioLightSettings& settings, JsonObject& root) {
    root["mode_id"] = settings.modeId;
  }

  static StateUpdateResult update(JsonObject& root, AudioLightSettings& settings) {
    settings.modeId = root["mode_id"] | AUDIO_LIGHT_DEFAULT_MODE;
    return StateUpdateResult::CHANGED;
  }
};

class AudioLightSettingsService : public StatefulService<AudioLightSettings> {
 public:
  AudioLightSettingsService(AsyncWebServer* server,
                            FS* fs,
                            SecurityManager* securityManager,
                            LedSettingsService* ledSettingsService,
                            FrequencySampler* frequencySampler);

  void begin();
  void loop();

 private:
  HttpEndpoint<AudioLightSettings> _httpEndpoint;
  AudioLightMode* _modes[NUM_MODES];
  AudioLightMode* _currentMode;

  StateUpdateResult update(JsonObject& root, AudioLightSettings& settings);
  AudioLightMode* getMode(const String& modeId);
  void sampleComplete();
  void saveModeConfig(AsyncWebServerRequest* request);
  void loadModeConfig(AsyncWebServerRequest* request);
};

#endif  // end AudioLightSettingsService_h