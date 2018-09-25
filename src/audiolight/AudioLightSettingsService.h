#ifndef AudioLightSettingsService_h
#define AudioLightSettingsService_h

#include "FastLED.h"
#include <SimpleSocket.h>
#include <SimpleService.h>
#include <audiolight/AudioLightMode.h>
#include <audiolight/ColorMode.h>
#include <audiolight/OffMode.h>

// fast led settings
#define LED_DATA_PIN 12
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LEDS 9
#define NUM_MODES 2

// 17ms delay gets us approximatly 60 samples per second
#define AUDIO_LIGHT_TICK_DELAY_MS 17

#define AUDIO_LIGHT_RESET_PIN 4
#define AUDIO_LIGHT_STROBE_PIN 5
#define AUDIO_LIGHT_ANALOG_PIN 0

#define AUDIO_LIGHT_WS_PATH "/ws/audioLight"
#define AUDIO_LIGHT_SERVICE_PATH "/rest/audioLight"
#define AUDIO_LIGHT_FREQUENCY_STREAM "/ws/frequencyStream"

class AudioLightSettingsService : public SimpleService, public SimpleSocket {

public:
  AudioLightSettingsService(AsyncWebServer *server, FS *fs);
  ~AudioLightSettingsService();

  void begin();
  void loop();

protected:
    void readFromJsonObject(JsonObject& root){
      readFromJsonObject(root, "service");
    }
    void readFromJsonObject(JsonObject& root, String originId);
    void writeToJsonObject(JsonObject& root);

private:
  // will serve setting endpoints from here
  AsyncWebServer* _server;

  // web socket for frequency broadcast
  AsyncWebSocket _webSocket;

  // The LED controller and led array
  CRGB _leds[NUM_LEDS];
  CLEDController *_ledController;

  // Construct modes
  AudioLightMode *_modes[NUM_MODES];
  AudioLightMode *_currentMode;

  // last tick tracker
  unsigned long _lastTickAt;

  // frequencies
  uint16_t _frequencies[7];

  // for FPS reporting
  unsigned long _lastReportedAt;
  uint8_t _numSamples;

  // buffer for writing to the web socket clients
  char _outputBuffer[37];  

  AudioLightMode* getMode(String modeId);
  void tick();
  void transmitFrequencies();
};

#endif // end AudioLightSettingsService_h