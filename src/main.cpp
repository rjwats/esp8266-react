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

#include <SecuritySettingsService.h>
#include <WiFiSettingsService.h>
#include <APSettingsService.h>
#include <NTPSettingsService.h>
#include <OTASettingsService.h>
#include <AuthenticationService.h>
#include <WiFiScanner.h>
#include <WiFiStatus.h>
#include <NTPStatus.h>
#include <APStatus.h>
#include <SystemStatus.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);

SecuritySettingsService securitySettingsService = SecuritySettingsService(&server, &SPIFFS);
WiFiSettingsService wifiSettingsService = WiFiSettingsService(&server, &SPIFFS, &securitySettingsService);
APSettingsService apSettingsService = APSettingsService(&server, &SPIFFS, &securitySettingsService);
NTPSettingsService ntpSettingsService = NTPSettingsService(&server, &SPIFFS, &securitySettingsService);
OTASettingsService otaSettingsService = OTASettingsService(&server, &SPIFFS, &securitySettingsService);
AuthenticationService authenticationService = AuthenticationService(&server, &securitySettingsService);

WiFiScanner wifiScanner = WiFiScanner(&server, &securitySettingsService);
WiFiStatus wifiStatus = WiFiStatus(&server, &securitySettingsService);
NTPStatus ntpStatus = NTPStatus(&server, &securitySettingsService);
APStatus apStatus = APStatus(&server, &securitySettingsService);
SystemStatus systemStatus = SystemStatus(&server, &securitySettingsService);;

void setup() {
    // Disable wifi config persistance and auto reconnect
    WiFi.persistent(false);
    WiFi.setAutoReconnect(false);

    Serial.begin(SERIAL_BAUD_RATE);
    SPIFFS.begin();

    // start security settings service first
    securitySettingsService.begin();

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
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", CORS_ORIGIN);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
    #endif

    server.begin();
}

void loop() {
  wifiSettingsService.loop();
  apSettingsService.loop();
  ntpSettingsService.loop();
  otaSettingsService.loop();
}
