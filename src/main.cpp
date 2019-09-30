#include <ESP8266React.h>
#include <AudioLightSettingsService.h>
#include <FS.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server, &SPIFFS);
AudioLightSettingsService audioLightService = AudioLightSettingsService(&server, &SPIFFS);

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();

  // set up the framework
  esp8266React.begin();

  // begin the demo project
  audioLightService.begin();
  
  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();

  // run the demo project's loop function
  audioLightService.loop();
}
