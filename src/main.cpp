#include <ESP8266React.h>
#include <DemoProject.h>
#include <FS.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React espServer(&SPIFFS);

DemoProject demoProject = DemoProject(&SPIFFS, espServer.getSecurityManager());

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();

  // set up the framework
  espServer.init(&server);

  // begin the demo project
  demoProject.init(&server);
  
  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  espServer.loop();

  // run the demo project's loop function
  demoProject.loop();
}
