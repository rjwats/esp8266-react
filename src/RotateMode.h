#ifndef RotateMode_h
#define RotateMode_h

#include <AudioLightMode.h>
#include <FrequencySampler.h>
#include <JsonUtil.h>

#ifndef FACTORY_ROTATE_MODE_DELAY
#define FACTORY_ROTATE_MODE_DELAY 30000
#endif

#define ROTATE_MODE_ID "rotate"

using ModeFetcher = std::function<AudioLightMode*(String& modeId)>;

class RotateModeSettings {
 public:
  std::list<String> modes;
  uint32_t delay;
};

class RotateMode : public AudioLightModeImpl<RotateModeSettings> {
 private:
  boolean _refresh = true;
  uint8_t _currentMode = 0;
  unsigned long _modeChangedAt = 0;
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
  bool canRotate();
};

#endif
