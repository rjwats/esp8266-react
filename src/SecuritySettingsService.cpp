#include <SecuritySettingsService.h>

SecuritySettingsService::SecuritySettingsService(AsyncWebServer* server, FS* fs) : AdminSettingsService(server, fs, this, SECURITY_SETTINGS_PATH, SECURITY_SETTINGS_FILE), SecurityManager() {}
SecuritySettingsService::~SecuritySettingsService() {}

void SecuritySettingsService::readFromJsonObject(JsonObject& root) {
  // secret  
  _jwtHandler.setSecret(root["jwt_secret"] | DEFAULT_JWT_SECRET);

  // users
  _users.clear();
  if (root["users"].is<JsonArray>()) {
    for (JsonVariant user :  root["users"].as<JsonArray>()) {
      _users.push_back(User(user["username"], user["password"], user["admin"]));
    }
  }
}

void SecuritySettingsService::writeToJsonObject(JsonObject& root) {
  // secret
  root["jwt_secret"] = _jwtHandler.getSecret();

  // users
  JsonArray users = root.createNestedArray("users");
  for (User _user : _users) {
    JsonObject user = users.createNestedObject();
    user["username"] = _user.getUsername();
    user["password"] = _user.getPassword();
    user["admin"] = _user.isAdmin();
  }
}

void SecuritySettingsService::begin() {
  readFromFS();
}
