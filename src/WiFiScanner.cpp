#include <WiFiScanner.h>

WiFiScanner::WiFiScanner(AsyncWebServer *server) : _server(server) {
  _server->on(SCAN_NETWORKS_SERVICE_PATH, HTTP_GET, std::bind(&WiFiScanner::scanNetworks, this, std::placeholders::_1));
  _server->on(LIST_NETWORKS_SERVICE_PATH, HTTP_GET, std::bind(&WiFiScanner::listNetworks, this, std::placeholders::_1));
}

void WiFiScanner::scanNetworks(AsyncWebServerRequest *request) {
      if (WiFi.scanComplete() != -1){
        WiFi.scanDelete();
        WiFi.scanNetworks(true);
      }
      request->send(202);
}

void WiFiScanner::listNetworks(AsyncWebServerRequest *request) {
  int numNetworks = WiFi.scanComplete();
  if (numNetworks > -1){
    AsyncJsonResponse * response = new AsyncJsonResponse();
    JsonObject& root = response->getRoot();
    JsonArray& networks = root.createNestedArray("networks");
    for (int i=0; i<numNetworks ; i++){
      JsonObject& network =  networks.createNestedObject();
      network["rssi"] = WiFi.RSSI(i);
      network["ssid"] = WiFi.SSID(i);
      network["bssid"] = WiFi.BSSIDstr(i);
      network["channel"] = WiFi.channel(i);
      network["encryption_type"] = WiFi.encryptionType(i);
      network["hidden"] = WiFi.isHidden(i);
    }
    response->setLength();
    request->send(response);
  } else if (numNetworks == -1){
    request->send(202);
  }else{
    scanNetworks(request);
  }
}
