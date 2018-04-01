#include <AuthSettingsService.h>

AuthSettingsService::AuthSettingsService(AsyncWebServer* server, FS* fs) : SettingsService(server, fs, AUTH_SETTINGS_SERVICE_PATH, AUTH_SETTINGS_FILE) {
  _server->on(AUTH_LOGOUT_SERVICE_PATH, HTTP_GET, std::bind(&AuthSettingsService::logout, this, std::placeholders::_1));

  // configure authentication handler
  _authenticationHandler.setUri(AUTH_AUTHENTICATE_SERVICE_PATH);
  _authenticationHandler.setMethod(HTTP_POST);
  _authenticationHandler.onRequest(std::bind(&AuthSettingsService::authenticate, this, std::placeholders::_1, std::placeholders::_2));
  _server->addHandler(&_authenticationHandler);
}

AuthSettingsService::~AuthSettingsService() {}

// checks the session is authenticated, refreshes the sessions timeout if so
bool AuthSettingsService::authenticated(AsyncWebServerRequest *request){
  request->send(400);
  return false;
}

void AuthSettingsService::readFromJsonObject(JsonObject& root){
  _username = root["username"] | AUTH_DEFAULT_USERNAME;
  _password = root["password"] | AUTH_DEFAULT_PASSWORD;
  _sessionTimeout= root["session_timeout"] | AUTH_DEFAULT_SESSION_TIMEOUT;
}

void AuthSettingsService::writeToJsonObject(JsonObject& root){
  root["username"] = _username;
  root["password"] = _password;
  root["session_timeout"] = _sessionTimeout;
}

void AuthSettingsService::logout(AsyncWebServerRequest *request){
  // revoke the current requests session
}

void AuthSettingsService::authenticate(AsyncWebServerRequest *request, JsonVariant &json){
  if (json.is<JsonObject>()){
    JsonObject& credentials = json.as<JsonObject>();
    if (credentials["username"] == _username && credentials["password"] == _password){
      // store cookie and write to response
    }
    request->send(401);
  } else{
    request->send(400);
  }
}
