#ifndef DemoProject_h
#define DemoProject_h

#include <AdminSettingsService.h>
#include <ESP8266React.h>

#define BLINK_LED 2
#define MAX_DELAY 1000
#define ECHO_CFG_DELAY 5000

#define DEFAULT_BLINK_SPEED 100
#define DEMO_SETTINGS_FILE "/config/demoSettings.json"
#define DEMO_SETTINGS_PATH "/rest/demoSettings"

class DemoProject : public AdminSettingsService {
 public:
  DemoProject(AsyncWebServer* server, FS* fs, ESP8266React* esp8266React);
  ~DemoProject();

  void loop();

 private:
  ESP8266React* _esp8266React;
  unsigned long _lastBlink = 0;
  unsigned long _lastEcho = 0;
  uint8_t _blinkSpeed = 255;

 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
};

#endif
