#ifndef SecurityManager_h
#define SecurityManager_h

#include <list>
#include <ArduinoJsonJWT.h>
#include <ESPAsyncWebServer.h>

#define DEFAULT_JWT_SECRET "esp8266-react"

#define AUTHORIZATION_HEADER "Authorization"
#define AUTHORIZATION_HEADER_PREFIX "Bearer "
#define AUTHORIZATION_HEADER_PREFIX_LEN 7

#define MAX_JWT_SIZE 128

class User {
  private:
    String _username;
    String _password;
    bool _admin;
  public:
    User(String username, String password, bool admin): _username(username), _password(password), _admin(admin) {}
    String getUsername() {
      return _username;
    }
    String getPassword() {
      return _password;
    }    
    bool isAdmin() {
      return _admin;
    }
};

class Authentication {
  private:
    User *_user;
    boolean _authenticated;
  public:
    Authentication(User& user): _user(new User(user)), _authenticated(true) {}
    Authentication() : _user(nullptr), _authenticated(false) {}  
    ~Authentication() {
      delete(_user);   
    }
    User* getUser() {
      return _user;
    }
    bool isAuthenticated() {
      return _authenticated;
    }
};

typedef std::function<boolean(Authentication &authentication)> AuthenticationPredicate;

class AuthenticationPredicates {
  public:
    static bool NONE_REQUIRED(Authentication &authentication) {
      return true;
    };
    static bool IS_AUTHENTICATED(Authentication &authentication) {
      return authentication.isAuthenticated();
    };    
    static bool IS_ADMIN(Authentication &authentication) {
      return authentication.isAuthenticated() && authentication.getUser()->isAdmin();
    };
};

class SecurityManager {

  public:

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

    /**
     * Wrap the provided request to provide validation against an AuthenticationPredicate.
     */
    ArRequestHandlerFunction wrapRequest(ArRequestHandlerFunction onRequest, AuthenticationPredicate predicate); 

  protected:

    ArduinoJsonJWT _jwtHandler = ArduinoJsonJWT(DEFAULT_JWT_SECRET);
    std::list<User> _users;

  private:

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