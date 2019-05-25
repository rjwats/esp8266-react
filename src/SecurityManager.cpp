#include <SecurityManager.h>

SecurityManager::SecurityManager(AsyncWebServer* server, FS* fs) :  SettingsService(server, fs, USERS_PATH, SECURITY_SETTINGS_FILE) {}
SecurityManager::~SecurityManager() {}

void SecurityManager::readFromJsonObject(JsonObject& root) {
  // secret  
  _jwtSecret = root["jwt_secret"] | DEFAULT_JWT_SECRET;

  // users
  _users.clear();
  if (root["users"].is<JsonArray>()) {
    for (JsonVariant user :  root["users"].as<JsonArray>()) {
      _users.push_back(User(user["username"], user["password"], user["admin"]));
    }
  }
}


void SecurityManager::writeToJsonObject(JsonObject& root) {
  // secret
  root["jwt_secret"] = _jwtSecret;

  // users
  JsonArray users = root.createNestedArray("users");
  for (User _user : _users) {
    JsonObject user = users.createNestedObject();
    user["username"] = _user.getUsername();
    user["password"] = _user.getPassword();
    user["admin"] = _user.isAdmin();
  }
}

void SecurityManager::begin() {
  // read config
  readFromFS();

  // configure secret
  jwtHandler.setSecret(_jwtSecret);
}

Authentication SecurityManager::authenticateRequest(AsyncWebServerRequest *request) {
  AsyncWebHeader* authorizationHeader = request->getHeader(AUTHORIZATION_HEADER);
  if (authorizationHeader) {
    String value = authorizationHeader->value();
    value.startsWith(AUTHORIZATION_HEADER_PREFIX);
    value = value.substring(AUTHORIZATION_HEADER_PREFIX_LEN);
    return authenticateJWT(value);
  }  
  return Authentication();
}

Authentication SecurityManager::authenticateJWT(String jwt) {
  DynamicJsonDocument payloadDocument(MAX_JWT_SIZE);
  jwtHandler.parseJWT(jwt, payloadDocument);
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
  return jwtHandler.buildJWT(payload);
}
