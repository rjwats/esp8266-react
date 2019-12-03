#ifndef AdminSettingsService_h
#define AdminSettingsService_h

#include <SettingsService.h>

class AdminSettingsService : public SettingsService {
 public:
  AdminSettingsService(AsyncWebServer* server,
                       FS* fs,
                       SecurityManager* securityManager,
                       char const* servicePath,
                       char const* filePath) :
      SettingsService(server, fs, servicePath, filePath),
      _securityManager(securityManager) {
  }

 protected:
  // will validate the requests with the security manager
  SecurityManager* _securityManager;

  void fetchConfig(AsyncWebServerRequest* request) {
    // verify the request against the predicate
    Authentication authentication = _securityManager->authenticateRequest(request);
    if (!getAuthenticationPredicate()(authentication)) {
      request->send(401);
      return;
    }
    // delegate to underlying implemetation
    SettingsService::fetchConfig(request);
  }

  void updateConfig(AsyncWebServerRequest* request, JsonDocument& jsonDocument) {
    // verify the request against the predicate
    Authentication authentication = _securityManager->authenticateRequest(request);
    if (!getAuthenticationPredicate()(authentication)) {
      request->send(401);
      return;
    }
    // delegate to underlying implemetation
    SettingsService::updateConfig(request, jsonDocument);
  }

  // override this to replace the default authentication predicate, IS_ADMIN
  AuthenticationPredicate getAuthenticationPredicate() {
    return AuthenticationPredicates::IS_ADMIN;
  }
};

#endif  // end AdminSettingsService
