#ifndef OTAUpload_h
#define OTAUpload_h

#include <Arduino.h>

#ifdef ESP32
#include <Update.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
#include <RestartService.h>

#define OTA_UPLOAD_PATH "/rest/otaUpload"

class OTAUpload {
 public:
  OTAUpload(AsyncWebServer* server, SecurityManager* securityManager);

 private:
  SecurityManager* _securityManager;
  void handleUpload(AsyncWebServerRequest* request,
                    const String& filename,
                    size_t index,
                    uint8_t* data,
                    size_t len,
                    bool final);
  void uploadComplete(AsyncWebServerRequest* request);
  void handleError(AsyncWebServerRequest* request, int code);
  static void handleEarlyDisconnect();
};

#endif  // end OTAUpload_h
