#ifndef SecuritySettingsService_h
#define SecuritySettingsService_h

#include <SecurityManager.h>
#include <HttpEndpoint.h>
#include <FSPersistence.h>

#define DEFAULT_ADMIN_USERNAME "admin"
#define DEFAULT_GUEST_USERNAME "guest"

#define SECURITY_SETTINGS_FILE "/config/securitySettings.json"
#define SECURITY_SETTINGS_PATH "/rest/securitySettings"

class SecuritySettings {
 public:
  String jwtSecret;
  std::list<User> users;

  static void serialize(SecuritySettings& settings, JsonObject& root) {
    // secret
    root["jwt_secret"] = settings.jwtSecret;

    // users
    JsonArray users = root.createNestedArray("users");
    for (User user : settings.users) {
      JsonObject userRoot = users.createNestedObject();
      userRoot["username"] = user.username;
      userRoot["password"] = user.password;
      userRoot["admin"] = user.admin;
    }
  }

  static void deserialize(JsonObject& root, SecuritySettings& settings) {
    // secret
    settings.jwtSecret = root["jwt_secret"] | DEFAULT_JWT_SECRET;

    // users
    settings.users.clear();
    if (root["users"].is<JsonArray>()) {
      for (JsonVariant user : root["users"].as<JsonArray>()) {
        settings.users.push_back(User(user["username"], user["password"], user["admin"]));
      }
    } else {
      settings.users.push_back(User(DEFAULT_ADMIN_USERNAME, DEFAULT_ADMIN_USERNAME, true));
      settings.users.push_back(User(DEFAULT_GUEST_USERNAME, DEFAULT_GUEST_USERNAME, false));
    }
  }
};

class SecuritySettingsService : public StatefulService<SecuritySettings>, public SecurityManager {
 public:
  SecuritySettingsService(AsyncWebServer* server, FS* fs);

  void begin();

  // Functions to implement SecurityManager
  Authentication authenticate(String& username, String& password);
  Authentication authenticateRequest(AsyncWebServerRequest* request);
  String generateJWT(User* user);
  ArRequestFilterFunction filterRequest(AuthenticationPredicate predicate);
  ArRequestHandlerFunction wrapRequest(ArRequestHandlerFunction onRequest, AuthenticationPredicate predicate);
  ArJsonRequestHandlerFunction wrapCallback(ArJsonRequestHandlerFunction callback, AuthenticationPredicate predicate);

 private:
  HttpEndpoint<SecuritySettings> _httpEndpoint;
  FSPersistence<SecuritySettings> _fsPersistence;
  ArduinoJsonJWT _jwtHandler = ArduinoJsonJWT(DEFAULT_JWT_SECRET);

  void configureJWTHandler();

  /*
   * Lookup the user by JWT
   */
  Authentication authenticateJWT(String& jwt);

  /*
   * Verify the payload is correct
   */
  boolean validatePayload(JsonObject& parsedPayload, User* user);
};

#endif  // end SecuritySettingsService_h