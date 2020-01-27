#ifndef SecurityManager_h
#define SecurityManager_h

#include <ArduinoJsonJWT.h>
#include <ESPAsyncWebServer.h>
#include <list>

#define DEFAULT_JWT_SECRET "esp8266-react"

#define AUTHORIZATION_HEADER "Authorization"
#define AUTHORIZATION_HEADER_PREFIX "Bearer "
#define AUTHORIZATION_HEADER_PREFIX_LEN 7

#define MAX_JWT_SIZE 128

class User {
 public:
  String username;
  String password;
  bool admin;

 public:
  User(String username, String password, bool admin) : username(username), password(password), admin(admin) {
  }
};

class Authentication {
 public:
  User* user;
  boolean authenticated;

 public:
  Authentication(User& user) : user(new User(user)), authenticated(true) {
  }
  Authentication() : user(nullptr), authenticated(false) {
  }
  ~Authentication() {
    delete (user);
  }
};

typedef std::function<boolean(Authentication& authentication)> AuthenticationPredicate;

class AuthenticationPredicates {
 public:
  static bool NONE_REQUIRED(Authentication& authentication) {
    return true;
  };
  static bool IS_AUTHENTICATED(Authentication& authentication) {
    return authentication.authenticated;
  };
  static bool IS_ADMIN(Authentication& authentication) {
    return authentication.authenticated && authentication.user->admin;
  };
};

class SecurityManager {
 public:
  /*
   * Authenticate, returning the user if found
   */
  virtual Authentication authenticate(String username, String password) = 0;

  /*
   * Check the request header for the Authorization token
   */
  virtual Authentication authenticateRequest(AsyncWebServerRequest* request) = 0;

  /*
   * Generate a JWT for the user provided
   */
  virtual String generateJWT(User* user) = 0;

  /**
   * Wrap the provided request to provide validation against an AuthenticationPredicate.
   */
  virtual ArRequestHandlerFunction wrapRequest(ArRequestHandlerFunction onRequest,
                                               AuthenticationPredicate predicate) = 0;
};

#endif  // end SecurityManager_h