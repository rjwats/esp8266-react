#include <Arduino.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
#endif

#include <FS.h>
#include <WiFiSettingsService.h>
#include <WiFiStatus.h>
#include <WiFiScanner.h>
#include <APSettingsService.h>
#include <NTPSettingsService.h>
#include <NTPStatus.h>
#include <OTASettingsService.h>
#include <APStatus.h>
#include <SecurityManager.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);

SecurityManager securityManager = SecurityManager(&server, &SPIFFS);

WiFiSettingsService wifiSettingsService = WiFiSettingsService(&server, &SPIFFS);
APSettingsService apSettingsService = APSettingsService(&server, &SPIFFS);
NTPSettingsService ntpSettingsService = NTPSettingsService(&server, &SPIFFS);
OTASettingsService otaSettingsService = OTASettingsService(&server, &SPIFFS);

WiFiScanner wifiScanner = WiFiScanner(&server);
WiFiStatus wifiStatus = WiFiStatus(&server);
NTPStatus ntpStatus = NTPStatus(&server);
APStatus apStatus = APStatus(&server);

void setup() {
    // Disable wifi config persistance
    WiFi.persistent(false);

    Serial.begin(SERIAL_BAUD_RATE);
    SPIFFS.begin();

    // start security manager
    securityManager.begin();

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
    server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");

    // Serving all other get requests with "/www/index.htm"
    // OPTIONS get a straight up 200 response
    server.onNotFound([](AsyncWebServerRequest *request) {
    	if (request->method() == HTTP_GET) {
        request->send(SPIFFS, "/www/index.html");
      } else if (request->method() == HTTP_OPTIONS) {
		    request->send(200);
      } else {
    		request->send(404);
    	}
    });

    // Disable CORS if required
    #if defined(ENABLE_CORS)
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
    #endif

    server.begin();
}

void loop() {
  apSettingsService.loop();
  ntpSettingsService.loop();
  otaSettingsService.loop();
}
