#ifndef AuthSettingsService_h
#define AuthSettingsService_h

#include <SettingsService.h>

#define AUTH_DEFAULT_USERNAME "admin"
#define AUTH_DEFAULT_PASSWORD "admin"
#define AUTH_DEFAULT_SESSION_TIMEOUT 3600

#define AUTH_SETTINGS_FILE "/config/authSettings.json"
#define AUTH_SETTINGS_SERVICE_PATH "/authSettings"

#define AUTH_LOGOUT_PATH "/logout"
#define AUTH_AUTHENTICATE_PATH "/authenticate"

// max number of concurrently authenticated clients
#define AUTH_MAX_CLIENTS 10

/*
* TODO: Will protect services with a cookie based authentication service.
*/

class AuthSettingsService : public SettingsService {

  public:

    AuthSettingsService(AsyncWebServer* server, FS* fs);
    ~AuthSettingsService();

    // checks the session is authenticated,
    // refreshes the sessions timeout if found
    bool authenticated(AsyncWebServerRequest *request);

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);

  private:

    // callback handler for authentication endpoint
    AsyncJsonRequestWebHandler _authenticationHandler;

    // only supporting one username at the moment
    String _username;
    String _password;

    // session timeout in seconds
    unsigned int _sessionTimeout;

    void logout(AsyncWebServerRequest *request);
    void authenticate(AsyncWebServerRequest *request, JsonVariant &json);

};

#endif // end AuthSettingsService_h
