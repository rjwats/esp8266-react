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
#include <audiolight/FireMode.h>
#include <audiolight/ConfettiMode.h>

// fast led settings
#define LED_DATA_PIN 12
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LEDS 9
#define NUM_MODES 7

#define OUTPUT_BUFFER_SIZE 7 * NUM_BANDS + 2

#define AUDIO_LIGHT_RESET_PIN 4
#define AUDIO_LIGHT_STROBE_PIN 5
#define AUDIO_LIGHT_ANALOG_PIN 36

#define AUDIO_LIGHT_DEAD_ZONE 700

// min and max for rolling avg factors
#define AUDIO_LIGHT_MAX_ROLLING_AVG_FACTOR 0.99
#define AUDIO_LIGHT_MIN_ROLLING_AVG_FACTOR 0.01
#define AUDIO_LIGHT_DEFAULT_ROLLING_AVG_FACTOR 0.15

#define AUDIO_LIGHT_WS_PATH "/ws/audioLight"
#define AUDIO_LIGHT_SERVICE_PATH "/rest/audioLight"
#define AUDIO_LIGHT_SAVE_MODE_CONFIG_PATH "/rest/saveAudioLightModeConfig"
#define AUDIO_LIGHT_LOAD_MODE_CONFIG_PATH "/rest/loadAudioLightModeConfig"
#define AUDIO_LIGHT_FREQUENCY_STREAM "/ws/frequencyStream"

class AudioLightSettingsService : public SimpleService, public SimpleSocket {

public:
  AudioLightSettingsService(AsyncWebServer *server, FS *fs);
  ~AudioLightSettingsService();

  void begin();
  void loop();

protected:

    struct AudioLightModeSettings {
      CLEDController *ledController;
      CRGB *leds;
      uint16_t numLeds;
      uint16_t *bands;
      uint16_t numBands;
      uint8_t brightness;
    };

    void readFromJsonObject(JsonObject& root){
      readFromJsonObject(root, "service");
    }
    
    // serialization functions
    void readFromJsonObject(JsonObject& root, String originId);
    void writeToJsonObject(JsonObject& root);

    // save and load the current mode config
    void saveModeConfig(AsyncWebServerRequest *request);
    void loadModeConfig(AsyncWebServerRequest *request);

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

  // frequencies and peaks
  uint16_t _bands[NUM_BANDS];
  uint16_t _rollingAverages[NUM_BANDS];

  // rolling average factor
  float _rollingAverageFactor = AUDIO_LIGHT_DEFAULT_ROLLING_AVG_FACTOR;

  // buffer for writing to the web socket clients
  char _outputBuffer[OUTPUT_BUFFER_SIZE];  

  AudioLightMode* getMode(String modeId);
  void transmitFrequencies();
};

#endif // end AudioLightSettingsService_h