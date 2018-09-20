#ifndef MSGEQ7SettingsService_h
#define MSGEQ7SettingsService_h

#include <SettingsService.h>

#define MSGEQ7_SAMPLE_FREQUENCY_MS 200

#define MSGEQ7_RESET_PIN 4
#define MSGEQ7_STROBE_PIN 5
#define MSGEQ7_ANALOG_PIN 0

#define MSGEQ7_SETTINGS_FILE "/config/msgeq7Settings.json"
#define MSGEQ7_SETTINGS_SERVICE_PATH "/rest/msgeq7Settings"

class MSGEQ7SettingsService : public SettingsService {

  public:

    MSGEQ7SettingsService(AsyncWebServer* server, FS* fs);
    ~MSGEQ7SettingsService();
    
    void begin();
    void loop();

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);

  private:

    unsigned long _lastSampledAt;
    uint16_t _samples[7];
    void sampleNow();

};

#endif // end MSGEQ7SettingsService_h