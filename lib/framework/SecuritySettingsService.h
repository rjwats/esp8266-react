#ifndef SecuritySettingsService_h
#define SecuritySettingsService_h

#include <AdminSettingsService.h>
#include <SecurityManager.h>

#define DEFAULT_ADMIN_USERNAME "admin"
#define DEFAULT_GUEST_USERNAME "guest"

#define SECURITY_SETTINGS_FILE "/config/securitySettings.json"
#define SECURITY_SETTINGS_PATH "/rest/securitySettings"

class SecuritySettingsService : public AdminSettingsService, public SecurityManager {
 public:
  SecuritySettingsService(AsyncWebServer* server, FS* fs);
  ~SecuritySettingsService();

 protected:
  void readFromJsonObject(JsonObject& root);
  void writeToJsonObject(JsonObject& root);
};

#endif  // end SecuritySettingsService_h