#include <SecuritySettingsService.h>

SecuritySettingsService::SecuritySettingsService(AsyncWebServer* server, FS* fs) :
    AdminSettingsService(server, fs, this, SECURITY_SETTINGS_PATH, SECURITY_SETTINGS_FILE),
    SecurityManager() {
}
SecuritySettingsService::~SecuritySettingsService() {
}

void SecuritySettingsService::readFromJsonObject(JsonObject& root) {
  // secret
  _jwtHandler.setSecret(root["jwt_secret"] | DEFAULT_JWT_SECRET);

  // users
  _settings.users.clear();
  if (root["users"].is<JsonArray>()) {
    for (JsonVariant user : root["users"].as<JsonArray>()) {
     _settings.users.push_back(User(user["username"], user["password"], user["admin"]));
    }
  } else {
    _settings.users.push_back(User(DEFAULT_ADMIN_USERNAME, DEFAULT_ADMIN_USERNAME, true));
    _settings.users.push_back(User(DEFAULT_GUEST_USERNAME, DEFAULT_GUEST_USERNAME, false));
  }
}

void SecuritySettingsService::writeToJsonObject(JsonObject& root) {
  // secret
  root["jwt_secret"] = _jwtHandler.getSecret();

  // users
  JsonArray users = root.createNestedArray("users");
  for (User _user : _settings.users) {
    JsonObject user = users.createNestedObject();
    user["username"] = _user.username;
    user["password"] = _user.password;
    user["admin"] = _user.admin;
  }
}


Authentication SecuritySettingsService::authenticateRequest(AsyncWebServerRequest *request) {
  AsyncWebHeader *authorizationHeader = request->getHeader(AUTHORIZATION_HEADER);
  if (authorizationHeader) {
    String value = authorizationHeader->value();
    if (value.startsWith(AUTHORIZATION_HEADER_PREFIX)) {
      value = value.substring(AUTHORIZATION_HEADER_PREFIX_LEN);
      return authenticateJWT(value);
    }
  }
  return Authentication();
}

Authentication SecuritySettingsService::authenticateJWT(String jwt) {
  DynamicJsonDocument payloadDocument(MAX_JWT_SIZE);
  _jwtHandler.parseJWT(jwt, payloadDocument);
  if (payloadDocument.is<JsonObject>()) {
    JsonObject parsedPayload = payloadDocument.as<JsonObject>();
    String username = parsedPayload["username"];
    for (User _user : _settings.users) {
      if (_user.username == username && validatePayload(parsedPayload, &_user)) {
        return Authentication(_user);
      }
    }
  }
  return Authentication();
}

Authentication SecuritySettingsService::authenticate(String username, String password) {
  for (User _user : _settings.users) {
    if (_user.username == username && _user.password == password) {
      return Authentication(_user);
    }
  }
  return Authentication();
}

inline void populateJWTPayload(JsonObject &payload, User *user) {
  payload["username"] = user->username;
  payload["admin"] = user->admin;
}

boolean SecuritySettingsService::validatePayload(JsonObject &parsedPayload, User *user) {
  DynamicJsonDocument _jsonDocument(MAX_JWT_SIZE);
  JsonObject payload = _jsonDocument.to<JsonObject>();
  populateJWTPayload(payload, user);
  return payload == parsedPayload;
}

String SecuritySettingsService::generateJWT(User *user) {
  DynamicJsonDocument _jsonDocument(MAX_JWT_SIZE);
  JsonObject payload = _jsonDocument.to<JsonObject>();
  populateJWTPayload(payload, user);
  return _jwtHandler.buildJWT(payload);
}

ArRequestHandlerFunction SecuritySettingsService::wrapRequest(ArRequestHandlerFunction onRequest,
                                                      AuthenticationPredicate predicate) {
  return [this, onRequest, predicate](AsyncWebServerRequest *request) {
    Authentication authentication = authenticateRequest(request);
    if (!predicate(authentication)) {
      request->send(401);
      return;
    }
    onRequest(request);
  };
}
