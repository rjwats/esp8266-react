#include <ESP8266React.h>

ESP8266React::ESP8266React(FS* fs): 
  _fs(fs),
  _securitySettingsService(_fs),
  _wifiSettingsService(_fs, &_securitySettingsService),
  _apSettingsService(_fs, &_securitySettingsService),
  _ntpSettingsService(_fs, &_securitySettingsService),
  _otaSettingsService(_fs, &_securitySettingsService),
  _authenticationService(&_securitySettingsService),
  _wifiScanner(&_securitySettingsService),
  _wifiStatus(&_securitySettingsService),
  _ntpStatus(&_securitySettingsService),
  _apStatus(&_securitySettingsService),
  _systemStatus(&_securitySettingsService) {
}

void ESP8266React::init(AsyncWebServer* server) {
    // Start security settings service first
  _securitySettingsService.init(server);

  // Core services
  _wifiSettingsService.init(server);
  _apSettingsService.init(server);
  _ntpSettingsService.init(server);
  _otaSettingsService.init(server);
  _authenticationService.init(server);

  // Utility services
  _wifiScanner.init(server);
  _wifiStatus.init(server);
  _ntpStatus.init(server);
  _apStatus.init(server);
  _systemStatus.init(server);


  // Serving static resources from /www/
  server->serveStatic("/js/", SPIFFS, "/www/js/");
  server->serveStatic("/css/", SPIFFS, "/www/css/");
  server->serveStatic("/fonts/", SPIFFS, "/www/fonts/");
  server->serveStatic("/app/", SPIFFS, "/www/app/");
  server->serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");

  // Serving all other get requests with "/www/index.htm"
  // OPTIONS get a straight up 200 response
  server->onNotFound([](AsyncWebServerRequest *request) {
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
}

void ESP8266React::loop() {  
  _wifiSettingsService.loop();
  _apSettingsService.loop();
  _ntpSettingsService.loop();
  _otaSettingsService.loop();
}
