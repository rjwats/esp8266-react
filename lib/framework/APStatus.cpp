#include <APStatus.h>

APStatus::APStatus(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(AP_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&APStatus::apStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
}

void APStatus::apStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_AP_STATUS_SIZE);
  JsonObject root = response->getRoot();

  WiFiMode_t currentWiFiMode = WiFi.getMode();
  root["active"] = (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA);
  root["ip_address"] = WiFi.softAPIP().toString();
  root["mac_address"] = WiFi.softAPmacAddress();
  root["station_num"] = WiFi.softAPgetStationNum();

  response->setLength();
  request->send(response);
}
