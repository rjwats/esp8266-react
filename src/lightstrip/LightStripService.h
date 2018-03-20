#ifndef LightStripService_h
#define LightStripService_h

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <FastLED.h>

#include <lightstrip/ColorMode.h>
#include <lightstrip/StrobeMode.h>
#include <lightstrip/OffMode.h>
#include <lightstrip/SmoothMode.h>
#include <lightstrip/FadeMode.h>
#include <lightstrip/FlashMode.h>
#include <lightstrip/IRCodes.h>
#include <lightstrip/Strip.h>
#include <lightstrip/SimpleService.h>

#define RECV_PIN 13
#define R_PIN 14
#define G_PIN 12
#define B_PIN 16

#define REPEAT_TIMEOUT_DURATION 120

#define LIGHT_STRIP_SETTINGS_SERVICE_PATH "/lightStrip"

class LightStripService : public SimpleService {

  public:

    LightStripService(AsyncWebServer* server);
    ~LightStripService();

    void loop();
    void begin();

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);

  private:

    IRrecv irrecv = IRrecv(RECV_PIN);

    Strip strip = Strip(R_PIN, G_PIN, B_PIN);

    ColorMode colorMode = ColorMode(&strip);
    SmoothMode smoothMode =SmoothMode(&strip);
    OffMode offMode = OffMode(&strip);
    FadeMode fadeMode = FadeMode(&strip);
    StrobeMode strobeMode = StrobeMode(&strip);
    FlashMode flashMode = FlashMode(&strip);

    Mode *currentMode = &offMode;

    // IR code handling
    long previousIRCode = 0;
    long repeatTimeout = 0;
    decode_results results;

    void changeMode(uint64_t modeCode);
    bool isModeCode(uint64_t code);
    Mode* getModeForCode(uint64_t code);
    void handleIRCode(uint64_t irCode, boolean repeat);
    void handleIRSensor();

};

#endif // end LightStripService_h
