#include <SecurityManager.h>

SecurityManager::SecurityManager(AsyncWebServer* server, FS* fs) : SettingsPersistence(fs, SECURITY_SETTINGS_FILE) {
  server->on(USERS_PATH, HTTP_GET, std::bind(&SecurityManager::fetchUsers, this, std::placeholders::_1));
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

void SecurityManager::fetchUsers(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse(MAX_USERS_SIZE);
  JsonObject jsonObject = response->getRoot();  
  writeToJsonObject(jsonObject);
  response->setLength();
  request->send(response);
}

void SecurityManager::begin() {
  readFromFS();
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
