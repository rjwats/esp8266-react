#include <ESP8266React.h>
#include <LightBrokerSettingsService.h>
#include <LightSettingsService.h>
#include <SensorSettingsService.h>
#include <SensorStatusService.h>
#include <FS.h>

#include "Wire.h"
#include "SPI.h"
#include <DHT.h>

#define DHTTYPE DHT22
#define DHTPIN 4

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server, &SPIFFS);
LightBrokerSettingsService lightBrokerSettingsService =
    LightBrokerSettingsService(&server, &SPIFFS, esp8266React.getSecurityManager());
LightSettingsService lightSettingsService = LightSettingsService(&server,
                                                                 esp8266React.getSecurityManager(),
                                                                 esp8266React.getMQTTClient(),
                                                                 &lightBrokerSettingsService);

SensorSettingsService sensorSettingsService =
    SensorSettingsService(&server, &SPIFFS, esp8266React.getSecurityManager());
SensorStatusService sensorStatusService = SensorStatusService(&server,
                                                              esp8266React.getSecurityManager(),
                                                              esp8266React.getMQTTClient(),
                                                              &sensorSettingsService);

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the file system (must be done before starting the framework)
#ifdef ESP32
  SPIFFS.begin(true);
#elif defined(ESP8266)
  SPIFFS.begin();
#endif

  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  lightSettingsService.begin();

  // start the light service
  lightBrokerSettingsService.begin();

  // load the initial sensor settings
  sensorSettingsService.begin();

    // start the sensor service
  sensorStatusService.begin();

  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();

  // run the sensor status main loop
  sensorStatusService.loop();
}
