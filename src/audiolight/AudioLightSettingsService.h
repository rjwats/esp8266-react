#ifndef AudioLightSettingsService_h
#define AudioLightSettingsService_h

#include "FastLED.h"
#include <SimpleSocket.h>
#include <SimpleService.h>

#include <audiolight/AudioLight.h>
#include <audiolight/AudioLightMode.h>
#include <audiolight/ColorMode.h>
#include <audiolight/OffMode.h>
#include <audiolight/SpectrumMode.h>
#include <audiolight/RainbowMode.h>
#include <audiolight/LightningMode.h>

// fast led settings
#define LED_DATA_PIN 12
#define COLOR_ORDER RGB
#define LED_TYPE WS2811
#define NUM_LEDS 50
#define NUM_MODES 5

#define OUTPUT_BUFFER_SIZE 7 * NUM_BANDS + 2

#define AUDIO_LIGHT_RESET_PIN 4
#define AUDIO_LIGHT_STROBE_PIN 5
#define AUDIO_LIGHT_ANALOG_PIN 36

#define DEAD_ZONE 256

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

  // frequencies and peaks
  uint16_t _bands[NUM_BANDS];

  // for FPS reporting
  unsigned long _lastReportedAt;
  uint16_t _numSamples;
  // the sampling delay
  uint16_t _samplingDelay;  

  // buffer for writing to the web socket clients
  char _outputBuffer[OUTPUT_BUFFER_SIZE];  

  AudioLightMode* getMode(String modeId);
  void transmitFrequencies();
};

#endif // end AudioLightSettingsService_h