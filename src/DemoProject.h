#ifndef DemoProject_h
#define DemoProject_h

#include <SettingsService.h>

#define BLINK_LED 2
#define MAX_DELAY 1000

#define DEFAULT_BLINK_SPEED 100
#define DEMO_SETTINGS_FILE "/config/demoSettings.json"
#define DEMO_SETTINGS_PATH "/rest/demoSettings"

class DemoProject : public AdminSettingsService {

  public:

    DemoProject(FS* fs, SecurityManager* securityManager) : AdminSettingsService(fs, securityManager, DEMO_SETTINGS_PATH, DEMO_SETTINGS_FILE) {}
    ~DemoProject() {}
    
    void init(AsyncWebServer* server);
    void loop();

  private:

    unsigned long _lastBlink = 0;
    uint8_t _blinkSpeed = 255;

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);
  
};

#endif
