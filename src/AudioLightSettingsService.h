#ifndef AudioLightSettingsService_h
#define AudioLightSettingsService_h

#include <ColorMode.h>
#include <StatefulService.h>
#include <HttpEndpoint.h>
#include <RainbowMode.h>
#include <LightningMode.h>
#include <ConfettiMode.h>
#include <FireMode.h>
#include <OffMode.h>
#include <PacificaMode.h>
#include <PrideMode.h>
#include <WebSocketTxRx.h>

#define NUM_MODES 8

#define AUDIO_LIGHT_SERVICE_PATH "/rest/audioLightSettings"
#define AUDIO_LIGHT_WS_PATH "/ws/audioLightSettings"

#define AUDIO_LIGHT_SAVE_MODE_PATH "/rest/saveModeSettings"
#define AUDIO_LIGHT_LOAD_MODE_PATH "/rest/loadModeSettings"

#define LOCAL_ORIGIN "local"

class AudioLightSettings {
 public:
  AudioLightMode* currentMode = nullptr;
};

class AudioLightSettingsService : public StatefulService<AudioLightSettings> {
 public:
  AudioLightSettingsService(AsyncWebServer* server,
                            FS* fs,
                            SecurityManager* securityManager,
                            LedSettingsService* ledSettingsService,
                            PaletteSettingsService* paletteSettingsService,
                            FrequencySampler* frequencySampler);

  void begin();
  void loop();

 private:
  HttpEndpoint<AudioLightSettings> _httpEndpoint;
  WebSocketTxRx<AudioLightSettings> _audioLightModeTxRx;
  AudioLightMode* _modes[NUM_MODES];

  void read(AudioLightSettings& settings, JsonObject& root);
  StateUpdateResult update(JsonObject& root, AudioLightSettings& settings);

  AudioLightMode* getMode(const String& modeId);
  void enableMode();
  void handleSample();
  void saveModeConfig(AsyncWebServerRequest* request);
  void loadModeConfig(AsyncWebServerRequest* request);
};

#endif  // end AudioLightSettingsService_h