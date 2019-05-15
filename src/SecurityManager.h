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
#define AUTHENTICATE_PATH "/rest/authenticate"
#define SIGN_IN_PATH "/rest/signIn"
#define TEST_VERIFICATION_PATH "/rest/verification"

#define MAX_JWT_SIZE 128
#define MAX_SECURITY_MANAGER_SETTINGS_SIZE 512
#define SECURITY_MANAGER_MAX_USERS 5

#define ANONYMOUS_USERNAME "_anonymous"
#define ANONYMOUS_PASSWORD ""
#define ANONYMOUS_ROLE ""

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

const User NOT_AUTHENTICATED = User(ANONYMOUS_USERNAME, ANONYMOUS_PASSWORD, ANONYMOUS_ROLE);

class Authentication {
  private:
    User _user;
    boolean _authenticated;
    Authentication(User user, boolean authenticated) : _user(user), _authenticated(authenticated) {}
  public:
    // NOOP
    ~Authentication(){}
    User& getUser() {
      return _user;
    }
    bool isAuthenticated() {
      return _authenticated;
    }
    static Authentication forUser(User user){
      return Authentication(user, true);
    }
    static Authentication notAuthenticated(){
      return Authentication(NOT_AUTHENTICATED, false);
    }
};

class SecurityManager :  public SettingsPersistence {

  public:

    SecurityManager(AsyncWebServer* server, FS* fs);
    ~SecurityManager();

    void begin();

    /*
    * Lookup the user by JWT
    */
    Authentication verify(String jwt);

    /*
    * Authenticate, returning the user if found.
    */
    Authentication authenticate(String username, String password);

    /*
    * Generate a JWT for the user provided
    */
    String generateJWT(User user);

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);

  private:
    // jwt handler
    ArduinoJsonJWT jwtHandler = ArduinoJsonJWT(DEFAULT_JWT_SECRET);

    // server instance
    AsyncWebServer* _server;
    AsyncJsonRequestWebHandler _signInRequestHandler;
    AsyncJsonRequestWebHandler _testVerifiction;

    // access point settings
    String _jwtSecret;
    std::list<String> _roles;
    std::list<User> _users;

    // endpoint functions
    void fetchUsers(AsyncWebServerRequest *request);
    void signIn(AsyncWebServerRequest *request, JsonDocument &jsonDocument);
    void testVerification(AsyncWebServerRequest *request, JsonDocument &jsonDocument);
};

#endif // end SecurityManager_h