#include <SecurityManager.h>

Authentication SecurityManager::authenticateRequest(AsyncWebServerRequest *request) {
  AsyncWebHeader* authorizationHeader = request->getHeader(AUTHORIZATION_HEADER);
  if (authorizationHeader) {
    String value = authorizationHeader->value();
    if (value.startsWith(AUTHORIZATION_HEADER_PREFIX)){
      value = value.substring(AUTHORIZATION_HEADER_PREFIX_LEN);
      return authenticateJWT(value);
    }   
  }  
  return Authentication();
}

Authentication SecurityManager::authenticateJWT(String jwt) {
  DynamicJsonDocument payloadDocument(MAX_JWT_SIZE);
  _jwtHandler.parseJWT(jwt, payloadDocument);
  if (payloadDocument.is<JsonObject>()) {
    JsonObject parsedPayload = payloadDocument.as<JsonObject>();
    String username = parsedPayload["username"];
    for (User _user : _users) {
      if (_user.getUsername() == username && validatePayload(parsedPayload, &_user)){
        return Authentication(_user);
      }
    }
  }
  return Authentication();
}

Authentication SecurityManager::authenticate(String username, String password) {
  for (User _user : _users) {
    if (_user.getUsername() == username && _user.getPassword() == password){
      return Authentication(_user);
    }
  }
  return Authentication();
}

inline void populateJWTPayload(JsonObject &payload, User *user) {
  payload["username"] = user->getUsername();
  payload["admin"] = user -> isAdmin();  
}

boolean SecurityManager::validatePayload(JsonObject &parsedPayload, User *user) {
  DynamicJsonDocument _jsonDocument(MAX_JWT_SIZE);      
  JsonObject payload = _jsonDocument.to<JsonObject>();
  populateJWTPayload(payload, user);
  return payload == parsedPayload;
}

String SecurityManager::generateJWT(User *user) {
  DynamicJsonDocument _jsonDocument(MAX_JWT_SIZE);      
  JsonObject payload = _jsonDocument.to<JsonObject>();
  populateJWTPayload(payload, user);
  return _jwtHandler.buildJWT(payload);
}

ArRequestHandlerFunction SecurityManager::wrapRequest(ArRequestHandlerFunction onRequest, AuthenticationPredicate predicate) {
  return [this, onRequest, predicate](AsyncWebServerRequest *request){
    Authentication authentication = authenticateRequest(request);
    if (!predicate(authentication)) {
      request->send(401);
      return;
    }
    onRequest(request);
  };
}    
    