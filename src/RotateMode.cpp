#include <RotateMode.h>

RotateMode::RotateMode(AsyncWebServer* server,
                       FS* fs,
                       SecurityManager* securityManager,
                       PaletteSettingsService* paletteSettingsService,
                       FrequencySampler* frequencySampler,
                       ModeFetcher modeFetcher) :
    AudioLightModeImpl(server,
                       fs,
                       securityManager,
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
  root["delay"] = settings.delay;
}

StateUpdateResult RotateMode::update(JsonObject& root, RotateModeSettings& settings) {
  settings.modes.clear();
  if (root["modes"].is<JsonArray>()) {
    for (String mode : root["modes"].as<JsonArray>()) {
      AudioLightMode* candidateMode = _modeFetcher(mode);
      if (candidateMode && candidateMode->canRotate()) {
        settings.modes.push_back(mode);
      }
    }
  }
  settings.delay = root["delay"] | FACTORY_ROTATE_MODE_DELAY;
  return StateUpdateResult::CHANGED;
}

void RotateMode::enable() {
  _refresh = true;
}

void RotateMode::tick(CRGB* leds, const uint16_t numLeds) {
  unsigned long currentMillis = millis();

  // refresh if we are required to
  if (_refresh) {
    selectMode(0, leds, numLeds);
    _modeChangedAt = millis();
    _refresh = false;
  }

  // hand off to the selected mode to do its thing
  if (_selectedMode) {
    _selectedMode->tick(leds, numLeds);
  }

  // change mode if it's time
  if ((unsigned long)(currentMillis - _modeChangedAt) >= _state.delay) {
    selectMode(_currentMode + 1, leds, numLeds);
    _modeChangedAt = millis();
  }
}

void RotateMode::selectMode(uint8_t newMode, CRGB* leds, const uint16_t numLeds) {
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
      fill_solid(leds, numLeds, CHSV(255, 0, 0));  // clear leds
      FastLED.show();                              // render all leds black
    }
  }
}

bool RotateMode::canRotate() {
  return false;
};
