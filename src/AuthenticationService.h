#ifndef AuthenticationService_H_
#define AuthenticationService_H_

#include <SecurityManager.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJsonWebHandler.h>
#include <AsyncArduinoJson6.h>

#define VERIFY_AUTHORIZATION_PATH "/rest/verifyAuthorization"
#define SIGN_IN_PATH "/rest/signIn"

#define MAX_AUTHENTICATION_SIZE 256

class AuthenticationService  {

  public:

    AuthenticationService(AsyncWebServer* server, SecurityManager* securityManager) ;
    ~AuthenticationService();

  private:
    // server instance 
    AsyncWebServer* _server;
    SecurityManager* _securityManager;
    AsyncJsonWebHandler _signInHandler;

    // endpoint functions
    void signIn(AsyncWebServerRequest *request, JsonDocument &jsonDocument);
    void verifyAuthorization(AsyncWebServerRequest *request);

};

#endif // end SecurityManager_h