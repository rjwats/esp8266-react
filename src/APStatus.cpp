#include <APStatus.h>

APStatus::APStatus(AsyncWebServer *server) : _server(server) {
  _server->on("/apStatus", HTTP_GET, std::bind(&APStatus::apStatus, this, std::placeholders::_1));
}

void APStatus::apStatus(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  JsonObject& root = response->getRoot();

  WiFiMode_t currentWiFiMode = WiFi.getMode();
  root["active"] =  (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA);
  root["ip_address"] = WiFi.softAPIP().toString();
  root["mac_address"] = WiFi.softAPmacAddress();
  root["station_num"] = WiFi.softAPgetStationNum();

  response->setLength();
  request->send(response);
}
