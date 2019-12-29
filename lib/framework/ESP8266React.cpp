#include <ESP8266React.h>

ESP8266React::ESP8266React(AsyncWebServer* server, FS* fs) :
    _securitySettingsService(server, fs),
    _wifiSettingsService(server, fs, &_securitySettingsService),
    _apSettingsService(server, fs, &_securitySettingsService),
    _ntpSettingsService(server, fs, &_securitySettingsService),
    _otaSettingsService(server, fs, &_securitySettingsService),
    _restartService(server, &_securitySettingsService),
    _authenticationService(server, &_securitySettingsService),
    _wifiScanner(server, &_securitySettingsService),
    _wifiStatus(server, &_securitySettingsService),
    _ntpStatus(server, &_securitySettingsService),
    _apStatus(server, &_securitySettingsService),
    _systemStatus(server, &_securitySettingsService) {
#ifdef PROGMEM_WWW
  // Serve static resources from PROGMEM
  WWWData::registerRoutes(
      [server, this](const String& uri, const String& contentType, const uint8_t* content, size_t len) {
        ArRequestHandlerFunction requestHandler = [contentType, content, len](AsyncWebServerRequest* request) {
          AsyncWebServerResponse* response = request->beginResponse_P(200, contentType, content, len);
          response->addHeader("Content-Encoding", "gzip");
          request->send(response);
        };
        server->on(uri.c_str(), HTTP_GET, requestHandler);
        // Serving non matching get requests with "/index.html"
        // OPTIONS get a straight up 200 response
        if (uri.equals("/index.html")) {
          server->onNotFound([requestHandler](AsyncWebServerRequest* request) {
            if (request->method() == HTTP_GET) {
              requestHandler(request);
            } else if (request->method() == HTTP_OPTIONS) {
              request->send(200);
            } else {
              request->send(404);
            }
          });
        }
      });
#else
  // Serve static resources from /www/
  server->serveStatic("/js/", SPIFFS, "/www/js/");
  server->serveStatic("/css/", SPIFFS, "/www/css/");
  server->serveStatic("/fonts/", SPIFFS, "/www/fonts/");
  server->serveStatic("/app/", SPIFFS, "/www/app/");
  server->serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");
  // Serving all other get requests with "/www/index.htm"
  // OPTIONS get a straight up 200 response
  server->onNotFound([](AsyncWebServerRequest* request) {
    if (request->method() == HTTP_GET) {
      request->send(SPIFFS, "/www/index.html");
    } else if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    }
  });
#endif

// Disable CORS if required
#if defined(ENABLE_CORS)
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", CORS_ORIGIN);
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
#endif
}

void ESP8266React::begin() {
  _securitySettingsService.begin();
  _wifiSettingsService.begin();
  _apSettingsService.begin();
  _ntpSettingsService.begin();
  _otaSettingsService.begin();
}

void ESP8266React::loop() {
  _wifiSettingsService.loop();
  _apSettingsService.loop();
  _ntpSettingsService.loop();
  _otaSettingsService.loop();
}
