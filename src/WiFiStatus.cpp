#include <WiFiStatus.h>

WiFiStatus::WiFiStatus(AsyncWebServer *server) : _server(server) {
  _server->on(WIFI_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&WiFiStatus::wifiStatus, this, std::placeholders::_1));

  _onStationModeConnectedHandler = WiFi.onStationModeConnected(onStationModeConnected);
  _onStationModeDisconnectedHandler = WiFi.onStationModeDisconnected(onStationModeDisconnected);
  _onStationModeGotIPHandler = WiFi.onStationModeGotIP(onStationModeGotIP);
}

void WiFiStatus::onStationModeConnected(const WiFiEventStationModeConnected& event) {
  Serial.print("WiFi Connected. SSID=");
  Serial.println(event.ssid);
}

void WiFiStatus::onStationModeDisconnected(const WiFiEventStationModeDisconnected& event) {
  Serial.print("WiFi Disconnected. Reason code=");
  Serial.println(event.reason);
}

void WiFiStatus::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.print("WiFi Got IP. localIP=");
  Serial.print(event.ip);
  Serial.print(", hostName=");
  Serial.println(WiFi.hostname());
}

void WiFiStatus::wifiStatus(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  JsonObject& root = response->getRoot();
  wl_status_t status = WiFi.status();
  root["status"] = (uint8_t) status;
  if (status == WL_CONNECTED){
    root["local_ip"] = WiFi.localIP().toString();
    root["rssi"] = WiFi.RSSI();
    root["ssid"] = WiFi.SSID();
    root["bssid"] = WiFi.BSSIDstr();
    root["channel"] = WiFi.channel();
    root["subnet_mask"] = WiFi.subnetMask().toString();
    root["gateway_ip"] = WiFi.gatewayIP().toString();
    IPAddress dnsIP1 = WiFi.dnsIP(0);
    IPAddress dnsIP2 = WiFi.dnsIP(1);
    if (dnsIP1 != 0U){
      root["dns_ip_1"] = dnsIP1.toString();
    }
    if (dnsIP2 != 0U){
      root["dns_ip_2"] = dnsIP2.toString();
    }
  }
  response->setLength();
  request->send(response);
}
