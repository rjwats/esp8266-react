#include <SecurityManager.h>

SecurityManager::SecurityManager(AsyncWebServer* server, FS* fs) : SettingsPersistence(fs, SECURITY_SETTINGS_FILE) {
  // fetch users
  server->on(USERS_PATH, HTTP_GET, std::bind(&SecurityManager::fetchUsers, this, std::placeholders::_1));

  // sign in request
  _signInRequestHandler.setUri(SIGN_IN_PATH);
  _signInRequestHandler.setMethod(HTTP_POST);
  _signInRequestHandler.setMaxContentLength(MAX_SECURITY_MANAGER_SETTINGS_SIZE);
  _signInRequestHandler.onRequest(std::bind(&SecurityManager::signIn, this, std::placeholders::_1, std::placeholders::_2));
  server->addHandler(&_signInRequestHandler);


    // sign in request
  _testVerifiction.setUri(TEST_VERIFICATION_PATH);
  _testVerifiction.setMethod(HTTP_POST);
  _testVerifiction.setMaxContentLength(MAX_SECURITY_MANAGER_SETTINGS_SIZE);
  _testVerifiction.onRequest(std::bind(&SecurityManager::testVerification, this, std::placeholders::_1, std::placeholders::_2));
  server->addHandler(&_testVerifiction);
}

SecurityManager::~SecurityManager() {}

void SecurityManager::readFromJsonObject(JsonObject& root) {
  // secret  
  _jwtSecret = root["jwt_secret"] | DEFAULT_JWT_SECRET;

  // roles
  _roles.clear();
  if (root["roles"].is<JsonArray>()) {
    JsonArray roles = root["roles"];
    for (JsonVariant role : roles) {
      _roles.push_back(role.as<String>());
    }
  }

  // users
  _users.clear();
  if (root["users"].is<JsonArray>()) {
    JsonArray users = root["users"];
    for (JsonVariant user : users) {
      String username =  user["username"];
      String password = user["password"];
      String role = user["role"];
      _users.push_back(User(username, password, role));
    }
  }
}

void SecurityManager::writeToJsonObject(JsonObject& root) {
  // secret
  root["jwt_secret"] = _jwtSecret;

  // roles
  JsonArray roles = root.createNestedArray("roles");
  for (String _role : _roles) {
    roles.add(_role);
  }

  // users
  JsonArray users = root.createNestedArray("users");
  for (User _user : _users) {
    JsonObject user = users.createNestedObject();
    user["username"] = _user.getUsername();
    user["password"] = _user.getPassword();
    user["role"] = _user.getRole();
  }
}

// TODO - Decide about default role behaviour, don't over-engineer (multiple roles, boolean admin flag???).
void SecurityManager::signIn(AsyncWebServerRequest *request, JsonDocument &jsonDocument){
  if (jsonDocument.is<JsonObject>()) {
    // authenticate user
    String username =  jsonDocument["username"];
    String password = jsonDocument["password"];
    User user = authenticate(username, password);

    if (user.isAuthenticated()) {
      // create JWT
      DynamicJsonDocument _jsonDocument(MAX_JWT_SIZE);      
      JsonObject jwt = _jsonDocument.to<JsonObject>();
      jwt["username"] = user.getUsername();
      jwt["role"] = user.getRole();
      
      // send JWT response
      AsyncJsonResponse * response = new AsyncJsonResponse(MAX_USERS_SIZE);
      JsonObject jsonObject = response->getRoot();
      jsonObject["access_token"] = jwtHandler.buildJWT(jwt);
      response->setLength();
      request->send(response);
      return;
    }
  }

  // authentication failed
  AsyncWebServerResponse *response =  request->beginResponse(401);
  request->send(response);
}

void SecurityManager::testVerification(AsyncWebServerRequest *request, JsonDocument &jsonDocument){
  if (jsonDocument.is<JsonObject>()) {    
    String accessToken =  jsonDocument["access_token"];
    DynamicJsonDocument parsedJwt(MAX_JWT_SIZE); 
    if (jwtHandler.parseJWT(accessToken, parsedJwt)){
      String username = parsedJwt["username"];

    }
  }
  // authentication failed
  AsyncWebServerResponse *response =  request->beginResponse(401);
  request->send(response);
}

void SecurityManager::fetchUsers(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse(MAX_USERS_SIZE);
  JsonObject jsonObject = response->getRoot();  
  writeToJsonObject(jsonObject);
  response->setLength();
  request->send(response);
}

void SecurityManager::begin() {
  // read config
  readFromFS();

  // configure secret
  jwtHandler.setSecret(_jwtSecret);
}

User SecurityManager::verifyUser(String jwt) {
  // TODO
  return NOT_AUTHENTICATED;
}

User SecurityManager::authenticate(String username, String password) {
  for (User _user : _users) {
    if (_user.getUsername() == username && _user.getPassword() == password){
      return _user;
    }
  }
  return NOT_AUTHENTICATED;
}

String SecurityManager::generateJWT(User user) {
  // TODO
  return "";
}
