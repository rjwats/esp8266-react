#ifndef SecurityManager_h
#define SecurityManager_h

#include <list>

#include <SettingsService.h>
#include <DNSServer.h>
#include <IPAddress.h>
#include <ArduinoJsonJWT.h>

#define DEFAULT_JWT_SECRET "esp8266-react"

#define SECURITY_SETTINGS_FILE "/config/securitySettings.json"

#define USERS_PATH "/rest/users"

#define AUTHORIZATION_HEADER "Authorization"
#define AUTHORIZATION_HEADER_PREFIX "Bearer "
#define AUTHORIZATION_HEADER_PREFIX_LEN 7

#define MAX_JWT_SIZE 128
#define MAX_SECURITY_MANAGER_SETTINGS_SIZE 512
#define SECURITY_MANAGER_MAX_USERS 5

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
};

class Authentication {
  private:
    User *_user;
    boolean _authenticated;
  public:
    Authentication(User& user): _user(new User(user)), _authenticated(true) {}
    Authentication() : _user(NULL), _authenticated(false) {}  
    ~Authentication() {
      if (_user != NULL){
        delete(_user);
      }      
    }
    User* getUser() {
      return _user;
    }
    bool isAuthenticated() {
      return _authenticated;
    }
};

class SecurityManager : public SettingsPersistence {

  public:

    SecurityManager(AsyncWebServer* server, FS* fs);
    ~SecurityManager();

    void begin();

    /*
    * Authenticate, returning the user if found
    */
    Authentication authenticate(String username, String password);

    /*
    * Check the request header for the Authorization token
    */
    Authentication authenticateRequest(AsyncWebServerRequest *request);
    
    /*
    * Generate a JWT for the user provided
    */
    String generateJWT(User *user);

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);

  private:
    // jwt handler
    ArduinoJsonJWT jwtHandler = ArduinoJsonJWT(DEFAULT_JWT_SECRET);

    // access point settings
    String _jwtSecret;
    std::list<String> _roles;
    std::list<User> _users;

    // endpoint functions
    void fetchUsers(AsyncWebServerRequest *request);

    /*
    * Lookup the user by JWT
    */
    Authentication authenticateJWT(String jwt);

    /*
    * Verify the payload is correct
    */
    boolean validatePayload(JsonObject &parsedPayload, User *user);

};

#endif // end SecurityManager_h