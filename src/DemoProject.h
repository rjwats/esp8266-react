#ifndef DemoProject_h
#define DemoProject_h

#define PTM(w) \
  Serial.print(" " #w "="); \
  Serial.print(tm->tm_##w);

#include <AdminSettingsService.h>
#include <sys/time.h>
#include <time.h>

#define BLINK_LED 2
#define MAX_DELAY 1000

#define DEFAULT_BLINK_SPEED 100
#define DEMO_SETTINGS_FILE "/config/demoSettings.json"
#define DEMO_SETTINGS_PATH "/rest/demoSettings"

class DemoProject : public AdminSettingsService {
 public:
  DemoProject(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  ~DemoProject();

  void loop();

 private:
  unsigned long _lastBlink = 0;
  unsigned long _lastTimeEcho = 0;
  uint8_t _blinkSpeed = 255;

 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
};

#endif
