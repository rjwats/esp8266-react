#ifndef SecuritySettingsService_h
#define SecuritySettingsService_h

#include <AdminSettingsService.h>
#include <SecurityManager.h>

#define DEFAULT_ADMIN_USERNAME "admin"
#define DEFAULT_GUEST_USERNAME "guest"

#define SECURITY_SETTINGS_FILE "/config/securitySettings.json"
#define SECURITY_SETTINGS_PATH "/rest/securitySettings"

#ifndef FT_SECURITY_DISABLED

class SecuritySettings {
 public:
  String jwtSecret;
  std::list<User> users;
};

class SecuritySettingsService : public AdminSettingsService<SecuritySettings>, public SecurityManager {
 public:
  SecuritySettingsService(AsyncWebServer* server, FS* fs);
  ~SecuritySettingsService();

  // Functions to implement SecurityManager
  Authentication authenticate(String username, String password);
  Authentication authenticateRequest(AsyncWebServerRequest* request);
  String generateJWT(User* user);
  ArRequestHandlerFunction wrapRequest(ArRequestHandlerFunction onRequest, AuthenticationPredicate predicate);

 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);

 private:
  ArduinoJsonJWT _jwtHandler = ArduinoJsonJWT(DEFAULT_JWT_SECRET);

  /*
   * Lookup the user by JWT
   */
  Authentication authenticateJWT(String jwt);

  /*
   * Verify the payload is correct
   */
  boolean validatePayload(JsonObject& parsedPayload, User* user);
};

#else

class SecuritySettingsService : public SecurityManager {
 public:
  SecuritySettingsService(AsyncWebServer* server, FS* fs);
  ~SecuritySettingsService();

  // minimal set of functions to support framework with security settings disabled
  Authentication authenticateRequest(AsyncWebServerRequest* request);
  ArRequestHandlerFunction wrapRequest(ArRequestHandlerFunction onRequest, AuthenticationPredicate predicate);
};

#endif // end FT_SECURITY_DISABLED
#endif // end SecuritySettingsService_h