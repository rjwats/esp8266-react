#include <APStatus.h>

APStatus::APStatus(APSettingsService* apSettingsService, AsyncWebServer* server, SecurityManager* securityManager) : 
    apSettingsService(apSettingsService) {
  server->on(AP_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&APStatus::apStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
}

void APStatus::apStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_AP_STATUS_SIZE);
  JsonObject root = response->getRoot();

  WiFiMode_t currentWiFiMode = WiFi.getMode();
  Status status;
  if ((currentWiFiMode & WIFI_AP) == 0) {
    status = INACTIVE;
  } else {
    uint8_t provisionMode = apSettingsService->getStateSnapshot().provisionMode;
    if (provisionMode == AP_MODE_ALWAYS 
        || (provisionMode == AP_MODE_DISCONNECTED && WiFi.status() != WL_CONNECTED)) {
      status = ACTIVE;
    } else {
      status = WAITING_CLIENTS_DISCONNECT;
    }
  }

  root["status"] = (uint8_t) status;
  root["ip_address"] = WiFi.softAPIP().toString();
  root["mac_address"] = WiFi.softAPmacAddress();
  root["station_num"] = WiFi.softAPgetStationNum();

  response->setLength();
  request->send(response);
}
