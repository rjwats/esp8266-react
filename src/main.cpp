#include <ESP8266React.h>
#include <DemoProject.h>
#include <FS.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React espServer(&SPIFFS);

DemoProject demoProject = DemoProject(&server, espServer.getSecurityManager());

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();

  // set up the framework
  espServer.init(&server);

  // begin the demo project
  demoProject.begin();
  
  server.begin();
}

void loop() {
  // run the framework loop
  espServer.loop();

  // run the demo project loop
  demoProject.loop();
}
