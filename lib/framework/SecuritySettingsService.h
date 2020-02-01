#ifndef SecuritySettingsService_h
#define SecuritySettingsService_h

#include <AdminSettingsService.h>
#include <SecurityManager.h>

#define DEFAULT_ADMIN_USERNAME "admin"
#define DEFAULT_GUEST_USERNAME "guest"

#define SECURITY_SETTINGS_FILE "/config/securitySettings.json"
#define SECURITY_SETTINGS_PATH "/rest/securitySettings"

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

#endif  // end SecuritySettingsService_h