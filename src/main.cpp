#include <ESP8266React.h>
#include <LedSettingsService.h>
#include <FrequencySampler.h>
#include <AudioLightSettingsService.h>
#include <FrequencyTransmitter.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);

LedSettingsService ledSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
PaletteSettingsService paletteSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
FrequencySampler frequencySampler(&ledSettingsService);
FrequencyTransmitter frequencyTransmitter(&server, esp8266React.getSecurityManager(), &frequencySampler);
AudioLightSettingsService audioLightSettingsService(&server,
                                                    esp8266React.getFS(),
                                                    esp8266React.getSecurityManager(),
                                                    &ledSettingsService,
                                                    &paletteSettingsService,
                                                    &frequencySampler);

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  // start the server
  server.begin();

  // set up the sampler
  frequencySampler.begin();

  // configure the LED strip
  ledSettingsService.begin();

  // load the palettes
  paletteSettingsService.begin();

  // load all of the defaults
  audioLightSettingsService.begin();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();

  // allow the sampler to run if needed
  frequencySampler.loop();

  // refresh the LEDs
  audioLightSettingsService.loop();
}
