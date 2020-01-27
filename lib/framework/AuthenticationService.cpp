#include <AuthenticationService.h>

AuthenticationService::AuthenticationService(AsyncWebServer* server, SecurityManager* securityManager) :
    _securityManager(securityManager) {
  server->on(VERIFY_AUTHORIZATION_PATH,
             HTTP_GET,
             std::bind(&AuthenticationService::verifyAuthorization, this, std::placeholders::_1));
  _signInHandler.setUri(SIGN_IN_PATH);
  _signInHandler.setMethod(HTTP_POST);
  _signInHandler.setMaxContentLength(MAX_AUTHENTICATION_SIZE);
  _signInHandler.onRequest(
      std::bind(&AuthenticationService::signIn, this, std::placeholders::_1, std::placeholders::_2));
  server->addHandler(&_signInHandler);
}

AuthenticationService::~AuthenticationService() {
}

/**
 * Verifys that the request supplied a valid JWT.
 */
void AuthenticationService::verifyAuthorization(AsyncWebServerRequest* request) {
  Authentication authentication = _securityManager->authenticateRequest(request);
  request->send(authentication.authenticated ? 200 : 401);
}

/**
 * Signs in a user if the username and password match. Provides a JWT to be used in the Authorization header in
 * subsequent requests.
 */
void AuthenticationService::signIn(AsyncWebServerRequest* request, JsonDocument& jsonDocument) {
  if (jsonDocument.is<JsonObject>()) {
    String username = jsonDocument["username"];
    String password = jsonDocument["password"];
    Authentication authentication = _securityManager->authenticate(username, password);
    if (authentication.authenticated) {
      User* user = authentication.user;
      AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_AUTHENTICATION_SIZE);
      JsonObject jsonObject = response->getRoot();
      jsonObject["access_token"] = _securityManager->generateJWT(user);
      response->setLength();
      request->send(response);
      return;
    }
  }
  AsyncWebServerResponse* response = request->beginResponse(401);
  request->send(response);
}
