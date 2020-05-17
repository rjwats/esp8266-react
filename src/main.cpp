#include <ESP8266React.h>
#include <LightMqttSettingsService.h>
#include <LightStateService.h>

#ifdef ESP32
#include <FS.h>
#define ESPFS SPIFFS
#elif defined(ESP8266)
#include <LittleFS.h>
#define ESPFS LittleFS
#endif

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server, &ESPFS);
LightMqttSettingsService lightMqttSettingsService =
    LightMqttSettingsService(&server, &ESPFS, esp8266React.getSecurityManager());
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &lightMqttSettingsService);

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the file system (must be done before starting the framework)
#ifdef ESP32
  ESPFS.begin(true);
#elif defined(ESP8266)
  ESPFS.begin();
#endif

  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  lightStateService.begin();

  // start the light service
  lightMqttSettingsService.begin();

  // start the server
  server.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();
}
