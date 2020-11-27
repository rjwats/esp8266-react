#include <RotateMode.h>

RotateMode::RotateMode(AsyncWebServer* server,
                       FS* fs,
                       SecurityManager* securityManager,
                       LedSettingsService* ledSettingsService,
                       PaletteSettingsService* paletteSettingsService,
                       FrequencySampler* frequencySampler,
                       ModeFetcher modeFetcher) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
                       ledSettingsService,
                       paletteSettingsService,
                       frequencySampler,
                       std::bind(&RotateMode::read, this, std::placeholders::_1, std::placeholders::_2),
                       std::bind(&RotateMode::update, this, std::placeholders::_1, std::placeholders::_2),
                       ROTATE_MODE_ID),
    _modeFetcher(modeFetcher) {
  addUpdateHandler([&](const String& originId) { enable(); }, false);
};

void RotateMode::read(RotateModeSettings& settings, JsonObject& root) {
  JsonArray modes = root.createNestedArray("modes");
  for (String mode : settings.modes) {
    modes.add(mode);
  }
}

StateUpdateResult RotateMode::update(JsonObject& root, RotateModeSettings& settings) {
  settings.modes.clear();
  if (root["modes"].is<JsonArray>()) {
    for (String mode : root["modes"].as<JsonArray>()) {
      // do not allow self - that would be bad!
      // check the mode exists
      if (mode != ROTATE_MODE_ID && _modeFetcher(mode)) {
        settings.modes.push_back(mode);
      }
    }
  }
  return StateUpdateResult::CHANGED;
}

void RotateMode::enable() {
  _refresh = true;
}

void RotateMode::tick() {
  if (_refresh) {
    selectMode(0);
    _refresh = false;
  }

  // hand off to the selected mode to do its thing
  if (_selectedMode) {
    _selectedMode->tick();
  }

  // select the next mode
  EVERY_N_MILLISECONDS(5000) {
    selectMode(_currentMode + 1);
  }
}

void RotateMode::selectMode(uint8_t newMode) {
  AudioLightMode* nextMode = nullptr;
  // select the next mode
  if (_state.modes.size() > 0) {
    // increment mode, resetting to zero if we've gone past the end of the array
    _currentMode = newMode < _state.modes.size() ? newMode : 0;
    // find the next mode
    nextMode = _modeFetcher(*std::next(_state.modes.begin(), _currentMode));
  }
  // activate the next mode if it's differnt from the selected mode
  if (nextMode != _selectedMode) {
    _selectedMode = nextMode;
    if (_selectedMode) {
      // new mode selected, enable it
      _selectedMode->enable();
    } else {
      // no mode selected, clear LEDs
      _ledSettingsService->update([&](CRGB* leds, CLEDController* ledController, const uint16_t numLeds) {
        fill_solid(leds, numLeds, CHSV(255, 0, 0));  // clear leds
        ledController->showLeds();                   // render all leds black
      });
    }
  }
}
