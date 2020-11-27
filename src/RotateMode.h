#ifndef RotateMode_h
#define RotateMode_h

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>

#define ROTATE_MODE_ID "rotate"

using ModeFetcher = std::function<AudioLightMode*(String& modeId)>;

class RotateModeSettings {
 public:
  std::list<String> modes;
};

class RotateMode : public AudioLightModeImpl<RotateModeSettings> {
 private:
  uint8_t _currentMode = 0;
  boolean _refresh = true;
  ModeFetcher _modeFetcher;
  AudioLightMode* _selectedMode;
  void selectMode(uint8_t modeOrdinal);

 public:
  RotateMode(AsyncWebServer* server,
             FS* fs,
             SecurityManager* securityManager,
             LedSettingsService* ledSettingsService,
             PaletteSettingsService* paletteSettingsService,
             FrequencySampler* frequencySampler,
             ModeFetcher modeFetcher);
  void tick();
  void enable();

  void read(RotateModeSettings& settings, JsonObject& root);
  StateUpdateResult update(JsonObject& root, RotateModeSettings& settings);
};

#endif
