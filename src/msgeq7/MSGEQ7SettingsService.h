#ifndef MSGEQ7SettingsService_h
#define MSGEQ7SettingsService_h

#include <SettingsService.h>

// TODO  - Remove me when effects have been factored out into modules
#include "FastLED.h"
#define LED_DATA_PIN 12
#define COLOR_ORDER GBR
#define LED_TYPE WS2812
#define NUM_LEDS 9

// 17ms delay gets us approximatly 60 samples per second
#define MSGEQ7_SAMPLE_DELAY_MS 17

#define MSGEQ7_RESET_PIN 4
#define MSGEQ7_STROBE_PIN 5
#define MSGEQ7_ANALOG_PIN 0

#define MSGEQ7_SETTINGS_FILE "/config/msgeq7Settings.json"
#define MSGEQ7_SETTINGS_SERVICE_PATH "/rest/msgeq7Settings"
#define MSGEQ7_FREQUENCY_STREAM "/ws/frequencyStream"

class MSGEQ7SettingsService : public SettingsService
{

public:
  MSGEQ7SettingsService(AsyncWebServer *server, FS *fs);
  ~MSGEQ7SettingsService();

  void begin();
  void loop();

protected:
  void readFromJsonObject(JsonObject &root);
  void writeToJsonObject(JsonObject &root);

private:
  AsyncWebSocket _webSocket;

  // frequency samples
  unsigned long _lastSampledAt;
  uint16_t _samples[7];

  // for FPS reporting
  unsigned long _lastReportedAt;
  uint8_t _numSamples;

  // buffer for writing to the web socket clients
  char _outputBuffer[37];

  // TEMP - hacking in the lightning effect
  struct CRGB leds[NUM_LEDS];
  uint8_t frequency = 50;
  uint8_t flashes = 8;
  unsigned int dimmer = 1;
  uint8_t ledstart;
  uint8_t ledlen;

  void sampleNow();
  void transmitFrequencies();
  void makeLightning();
};

#endif // end MSGEQ7SettingsService_h