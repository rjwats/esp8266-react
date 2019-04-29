#include <SecurityManager.h>

SecurityManager::SecurityManager(AsyncWebServer* server, FS* fs) : SettingsPersistence(fs, SECURITY_SETTINGS_FILE) {
}

SecurityManager::~SecurityManager() {}

void SecurityManager::readFromJsonObject(JsonObject& root) {
  _jwtSecret = root["jwt_secret"] | DEFAULT_JWT_SECRET;

  while (_numUsers > 0){
    delete _users[--_numUsers];
  }

  if (root["users"].is<JsonArray>()) {
    JsonArray users = root["users"];
    _numUsers = 0;
    // TODO - complete defence against bad data
    for (int i =0; i < min(SECURITY_MANAGER_MAX_USERS, (int) users.size()); i++){
      JsonObject user = users[i];
      String username =  user["username"];;
      String password = user["password"];
      String role = user["role"];
      _users[_numUsers++] = new User(username, password, role);
    }
  }
}

void SecurityManager::writeToJsonObject(JsonObject& root) {
  // TODO
}

void SecurityManager::begin() {
  // TODO
}

User SecurityManager::verifyUser(String jwt) {
  // TODO
  return NOT_AUTHENTICATED;
}
User authenticate(String username, String password) {
  // TODO
  return NOT_AUTHENTICATED;
}

String generateJWT(User user) {
  // TODO
  return "";
}


