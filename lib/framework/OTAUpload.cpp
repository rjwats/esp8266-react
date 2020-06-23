#include <OTAUpload.h>

OTAUpload::OTAUpload(AsyncWebServer* server, SecurityManager* securityManager) : _securityManager(securityManager) {
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
#ifdef ESP8266
  Update.runAsync(true);
#endif
}

void OTAUpload::handleUpload(AsyncWebServerRequest* request,
                             const String& filename,
                             size_t index,
                             uint8_t* data,
                             size_t len,
                             bool final) {
  if (!index) {
    Authentication authentication = _securityManager->authenticateRequest(request);
    if (AuthenticationPredicates::IS_ADMIN(authentication)) {
      if (Update.begin(request->contentLength())) {
        // it is possible for the client to hang up
        // let's make sure we end the update if that happens
        request->onDisconnect([]() { Update.end(); });
      } else {
        Update.printError(Serial);
        handleError(request, UpdateError::BEGIN_ERROR, 500);
      }
    } else {
      handleError(request, UpdateError::BEGIN_ERROR, 403);
    }
  }

  // we must call end() when we receive the final frame, so we continue on update errors
  UpdateError* error = (UpdateError*)request->_tempObject;
  if (!error || *error == UpdateError::UPDATE_ERROR) {
    if (Update.write(data, len) != len) {
      Update.printError(Serial);
      handleError(request, UpdateError::UPDATE_ERROR, 500);
    }
    if (final) {
      if (!Update.end(true)) {
        Update.printError(Serial);
        handleError(request, UpdateError::UPDATE_ERROR, 500);
      }
    }
  }
}

void OTAUpload::uploadComplete(AsyncWebServerRequest* request) {
  // if we encountered an error, don't render a response
  UpdateError* error = (UpdateError*)request->_tempObject;
  if (error) {
    return;
  }

  // perform restart on disconnect & send 200 response
  request->onDisconnect([]() { RestartService::restartNow(); });
  AsyncWebServerResponse* response = request->beginResponse(200);
  request->send(response);
}

void OTAUpload::handleError(AsyncWebServerRequest* request, UpdateError error, int code) {
  // we have already delt with an error during the update request do nothing
  if (request->_tempObject != nullptr) {
    return;
  }

  // send the error code to the client and record the error in the temp object
  request->_tempObject = new UpdateError(error);
  AsyncWebServerResponse* response = request->beginResponse(code);
  request->send(response);
}
