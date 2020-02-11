#ifndef AuthenticationService_H_
#define AuthenticationService_H_

#include <Features.h>
#include <AsyncJson.h>
#include <AsyncJsonWebHandler.h>
#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>

#define VERIFY_AUTHORIZATION_PATH "/rest/verifyAuthorization"
#define SIGN_IN_PATH "/rest/signIn"

#define MAX_AUTHENTICATION_SIZE 256

#if USING(FT_SECURITY)

class AuthenticationService {
 public:
  AuthenticationService(AsyncWebServer* server, SecurityManager* securityManager);
  ~AuthenticationService();

 private:
  SecurityManager* _securityManager;
  AsyncJsonWebHandler _signInHandler;

  // endpoint functions
  void signIn(AsyncWebServerRequest* request, JsonDocument& jsonDocument);
  void verifyAuthorization(AsyncWebServerRequest* request);
};

#endif  // end USING(FT_SECURITY)
#endif  // end SecurityManager_h