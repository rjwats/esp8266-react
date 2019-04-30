#ifndef SecurityManager_h
#define SecurityManager_h

#include <list>
#include <ArduinoJWT.h>

#include <SettingsService.h>
#include <DNSServer.h>
#include <IPAddress.h>

#define DEFAULT_JWT_SECRET "esp8266-react"

#define SECURITY_SETTINGS_FILE "/config/securitySettings.json"

#define USERS_PATH "/rest/users"
#define AUTHENTICATE_PATH "/rest/authenticate"

#define SECURITY_MANAGER_MAX_USERS 5

#define UNAUTHENTICATED_USERNAME "_anonymous"
#define UNAUTHENTICATED_PASSWORD ""
#define UNAUTHENTICATED_ROLE ""

#define MAX_USERS_SIZE 1024

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
};

const User NOT_AUTHENTICATED = User(UNAUTHENTICATED_USERNAME, UNAUTHENTICATED_PASSWORD, UNAUTHENTICATED_ROLE);

class SecurityManager :  public SettingsPersistence {

  public:

    SecurityManager(AsyncWebServer* server, FS* fs);
    ~SecurityManager();

    void begin();

    User verifyUser(String jwt);
    User authenticate(String username, String password);
    String generateJWT(User user);

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);

  private:

    // server instance
    AsyncWebServer* _server;

    // access point settings
    String _jwtSecret;
    std::list<String> _roles;
    std::list<User> _users;

    // endpoint functions
    void fetchUsers(AsyncWebServerRequest *request);

};

#endif // end SecurityManager_h