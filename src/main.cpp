#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <WiFiSettingsService.h>
#include <WiFiStatus.h>
#include <WiFiScanner.h>
#include <APSettingsService.h>
#include <NTPSettingsService.h>
#include <NTPStatus.h>
#include <OTASettingsService.h>
#include <APStatus.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);

WiFiSettingsService wifiSettingsService = WiFiSettingsService(&server, &SPIFFS);
WiFiStatus wifiStatus = WiFiStatus(&server);
WiFiScanner wifiScanner = WiFiScanner(&server);
APSettingsService apSettingsService = APSettingsService(&server, &SPIFFS);
NTPSettingsService ntpSettingsService = NTPSettingsService(&server, &SPIFFS);
OTASettingsService otaSettingsService = OTASettingsService(&server, &SPIFFS);
NTPStatus ntpStatus = NTPStatus(&server);
APStatus apStatus = APStatus(&server);

void setup() {
    // Disable wifi config persistance
    WiFi.persistent(false);

    Serial.begin(SERIAL_BAUD_RATE);
    SPIFFS.begin();

    // start services
    ntpSettingsService.begin();
    otaSettingsService.begin();
    apSettingsService.begin();
    wifiSettingsService.begin();

    // Serving static resources from /www/
    server.serveStatic("/js/", SPIFFS, "/www/js/");
    server.serveStatic("/css/", SPIFFS, "/www/css/");
    server.serveStatic("/fonts/", SPIFFS, "/www/fonts/");
    server.serveStatic("/app/", SPIFFS, "/www/app/");

    // Serving all other get requests with "/www/index.htm"
    server.onNotFound([](AsyncWebServerRequest *request) {
    	if (request->method() == HTTP_GET) {
        request->send(SPIFFS, "/www/index.html");
      } else {
    		request->send(404);
    	}
    });

    server.begin();
}

void loop() {
  apSettingsService.loop();
  ntpSettingsService.loop();
  otaSettingsService.loop();
}
