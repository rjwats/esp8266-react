#include <ESP8266React.h>
#include <DemoProject.h>
#include <Bme280Project.h>
#include <FS.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server, &SPIFFS);
DemoProject demoProject = DemoProject(&server, &SPIFFS, esp8266React.getSecurityManager());
// Bme280Project bme280Project = Bme280Project(&server);

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the file system (must be done before starting the framework)
  SPIFFS.begin();

  // start the framework and demo project
  esp8266React.begin();

  // start the demo project
  demoProject.begin();

  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();

  // run the demo project's loop function
  demoProject.loop();

  // run the bme280 project's loop function
  // bme280Project.loop();
}
