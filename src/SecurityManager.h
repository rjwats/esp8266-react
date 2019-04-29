#ifndef APSettingsConfig_h
#define APSettingsConfig_h

#include <SettingsService.h>
#include <DNSServer.h>
#include <IPAddress.h>

#define DEFAULT_JWT_SECRET "esp8266-react"

#define SECURITY_SETTINGS_FILE "/config/securitySettings.json"
#define AUTHENTICATE_PATH "/rest/authenticate"

#define SECURITY_MANAGER_MAX_USERS 5

#define UNAUTHENTICATED_USERNAME ""
#define UNAUTHENTICATED_PASSWORD ""
#define UNAUTHENTICATED_ROLE ""

#define ROLE_ADMIN "admin"
#define ROLE_GUEST "guest"

class User {
  private:
    String _username;
    String _password;
    String _role;
  public:
    User(String username, String password, String role): _username(username), _password(password), _role(role) {}
    String getUsername() {
      return _username;
    }
    String getPassword() {
      return _password;
    }    
    String getRole() {
      return _role;
    }
    bool isAuthenticated() {
      return _username != UNAUTHENTICATED_USERNAME;
    }
    bool isAdmin() {
      return isAuthenticated() && _username == ROLE_ADMIN;
    }
};

const User NOT_AUTHENTICATED = User(UNAUTHENTICATED_USERNAME, UNAUTHENTICATED_PASSWORD, UNAUTHENTICATED_ROLE);

class SecurityManager :  public SettingsPersistence {

  public:

    SecurityManager(AsyncWebServer* server, FS* fs);
    ~SecurityManager();

    void begin();

    User verifyUser(String jwt);
    User authenticate();
    String generateJWT(User user);

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);

  private:

    // access point settings
    String _jwtSecret;
    User *_users[SECURITY_MANAGER_MAX_USERS];
    int _numUsers;
};

#endif // end APSettingsConfig_h