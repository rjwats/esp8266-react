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
#if defined(ESP8266)
      network["encryption_type"] = convertEncryptionType(WiFi.encryptionType(i));
#elif defined(ESP_PLATFORM)
      network["encryption_type"] = (uint8_t) WiFi.encryptionType(i); 
#endif
    }
    response->setLength();
    request->send(response);
  } else if (numNetworks == -1){
    request->send(202);
  }else{
    scanNetworks(request);
  }
}

#if defined(ESP8266)
/*
 * Convert encryption type to standard used by ESP32 rather than the translated form which the esp8266 libaries expose.
 * 
 * This allows us to use a single set of mappings in the UI.
 */
uint8_t WiFiScanner::convertEncryptionType(uint8_t encryptionType){
  switch (encryptionType){
    case ENC_TYPE_NONE:
      return AUTH_OPEN;
    case ENC_TYPE_WEP:
      return AUTH_WEP;
    case ENC_TYPE_TKIP:
      return AUTH_WPA_PSK;
    case ENC_TYPE_CCMP:
      return AUTH_WPA2_PSK;
    case ENC_TYPE_AUTO:
      return AUTH_WPA_WPA2_PSK;
  }
  return -1;
}
#endif