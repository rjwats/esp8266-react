#ifndef AsyncAuthJsonWebHandler_H_
#define AsyncAuthJsonWebHandler_H_

#include <ESPAsyncWebServer.h>
#include <AsyncJsonWebHandler.h>
#include <ArduinoJson.h>
#include <SecurityManager.h>

typedef std::function<void(AsyncWebServerRequest *request, JsonDocument &jsonDocument, Authentication &authentication)> AuthenticationJsonRequestCallback;

/**
 * Extends AsyncJsonWebHandler with a wrapper which verifies the user is authenticated.
 * 
 * TODO - Extend with role checking support, possibly with a callback to verify the user.
 */
class AsyncAuthJsonWebHandler: public AsyncJsonWebHandler {

  private:
    SecurityManager *_securityManager;
    using AsyncJsonWebHandler::onRequest;
    
  public:

    AsyncAuthJsonWebHandler() : 
      AsyncJsonWebHandler(), _securityManager(NULL) {}

    ~AsyncAuthJsonWebHandler() {}

    void setSecurityManager(SecurityManager *securityManager) { 
      _securityManager = securityManager;
    }

    void onRequest(AuthenticationJsonRequestCallback callback) {
      AsyncJsonWebHandler::onRequest([this, callback](AsyncWebServerRequest *request, JsonDocument &jsonDocument) {
        if(!_securityManager) {
          Serial.print("Security manager not configured for endpoint: ");
          Serial.println(_uri);           
          request->send(500);
          return;
        }
        Authentication authentication = _securityManager->authenticateRequest(request);
        if (!authentication.isAuthenticated()) {
          request->send(401);
          return;
        }
        callback(request, jsonDocument, authentication);
      });
    }

};

#endif // end AsyncAuthJsonWebHandler_H_