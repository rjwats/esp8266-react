#include <ESP8266React.h>
#include <DemoProject.h>
#include <FS.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React framework(&server, &SPIFFS);
DemoProject demoProject = DemoProject(&server, &SPIFFS, framework.getSecurityManager());

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();
  
  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  framework.loop();

  // run the demo project's loop function
  demoProject.loop();
}
