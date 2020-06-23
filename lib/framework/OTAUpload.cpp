#include <OTAUpload.h>

OTAUpload::OTAUpload(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(OTA_UPLOAD_PATH,
             HTTP_POST,
             std::bind(&OTAUpload::uploadComplete, this, std::placeholders::_1),
             std::bind(&OTAUpload::handleUpload,
                       this,
                       std::placeholders::_1,
                       std::placeholders::_2,
                       std::placeholders::_3,
                       std::placeholders::_4,
                       std::placeholders::_5,
                       std::placeholders::_6));
}

void OTAUpload::uploadComplete(AsyncWebServerRequest* request) {
  bool success = Update.isFinished() && !Update.hasError();

  // restart on disconnect
  if (success) {
    request->onDisconnect([]() { RestartService::restartNow(); });
  }

  // send the response
  AsyncWebServerResponse* response = request->beginResponse(success ? 200 : 500);
  request->send(response);
}

void OTAUpload::handleUpload(AsyncWebServerRequest* request,
                             const String& filename,
                             size_t index,
                             uint8_t* data,
                             size_t len,
                             bool final) {
  if (!index) {
#ifdef ESP8266
    Update.runAsync(true);
#endif
    if (!Update.begin(request->contentLength())) {
      Update.printError(Serial);
    }
  }
  if (Update.isRunning()) {
    if (Update.write(data, len) != len) {
      Update.printError(Serial);
    }
    if (final) {
      Update.end(true);
    }
  }
}
