#ifndef AuthenticationService_H_
#define AuthenticationService_H_

#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>

#define VERIFY_AUTHORIZATION_PATH "/rest/verifyAuthorization"
#define SIGN_IN_PATH "/rest/signIn"

#define MAX_AUTHENTICATION_SIZE 256

class AuthenticationService {
 public:
  AuthenticationService(AsyncWebServer* server, SecurityManager* securityManager);

 private:
  SecurityManager* _securityManager;
  AsyncCallbackJsonWebHandler _signInHandler;

  // endpoint functions
  void signIn(AsyncWebServerRequest* request, JsonVariant& json);
  void verifyAuthorization(AsyncWebServerRequest* request);
};

#endif  // end SecurityManager_h