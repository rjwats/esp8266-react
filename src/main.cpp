#include <ESP8266React.h>
#include <AudioLightSettingsService.h>
#include <FS.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React framework(&SPIFFS);
AudioLightSettingsService audioLightService = AudioLightSettingsService(&server, &SPIFFS);

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();

  // set up the framework
  framework.init(&server);

  // begin the demo project
  audioLightService.init(&server);
  audioLightService.begin();
  
  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  framework.loop();

  // run the demo project's loop function
  audioLightService.loop();
}
