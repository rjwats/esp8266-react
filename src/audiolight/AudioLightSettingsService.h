#ifndef AudioLightSettingsService_h
#define AudioLightSettingsService_h

#include "FastLED.h"
#include <SettingsService.h>
#include <audiolight/ColorMode.h>
#include <audiolight/OffMode.h>

// fast led settings
#define LED_DATA_PIN 12
#define COLOR_ORDER GBR
#define LED_TYPE WS2812
#define NUM_LEDS 9

// 17ms delay gets us approximatly 60 samples per second
#define AUDIO_LIGHT_TICK_DELAY_MS 17

#define AUDIO_LIGHT_RESET_PIN 4
#define AUDIO_LIGHT_STROBE_PIN 5
#define AUDIO_LIGHT_ANALOG_PIN 0

#define AUDIO_LIGHT_SETTINGS_FILE "/config/audioLight.json"
#define AUDIO_LIGHT_SETTINGS_SERVICE_PATH "/rest/audioLight"
#define AUDIO_LIGHT_FREQUENCY_STREAM "/ws/frequencyStream"

class AudioLightSettingsService : public SettingsService
{

public:
  AudioLightSettingsService(AsyncWebServer *server, FS *fs);
  ~AudioLightSettingsService();

  void begin();
  void loop();

protected:
  void readFromJsonObject(JsonObject &root);
  void writeToJsonObject(JsonObject &root);

private:
  AsyncWebSocket _webSocket;

  // The LED controller and led array
  CRGB _leds[NUM_LEDS];
  CLEDController *_ledController = &FastLED.addLeds<LED_TYPE,LED_DATA_PIN>(_leds, NUM_LEDS);

  // Construct modes
  OffMode _offMode = OffMode(_ledController, _leds, _frequencies, NUM_LEDS);
  ColorMode _colorMode = ColorMode(_ledController, _leds, _frequencies, NUM_LEDS);

  // last tick tracker
  unsigned long _lastTickAt;

  // frequencies
  uint16_t _frequencies[7];

  // for FPS reporting
  unsigned long _lastReportedAt;
  uint8_t _numSamples;

  // buffer for writing to the web socket clients
  char _outputBuffer[37];  

  void tick();
  void transmitFrequencies();
};

#endif // end AudioLightSettingsService_h