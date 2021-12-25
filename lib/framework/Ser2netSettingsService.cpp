#include <Ser2netSettingsService.h>

Ser2netSettingsService::Ser2netSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(Ser2netSettings::read, Ser2netSettings::update, this, server, SER2NET_SETTINGS_SERVICE_PATH, securityManager),
    _fsPersistence(Ser2netSettings::read, Ser2netSettings::update, this, fs, SER2NET_SETTINGS_FILE),
    _timeHandler(TIME_PATH,
                 securityManager->wrapCallback(
                     std::bind(&Ser2netSettingsService::configureTime, this, std::placeholders::_1, std::placeholders::_2),
                     AuthenticationPredicates::IS_ADMIN)) {
  _timeHandler.setMethod(HTTP_POST);
  _timeHandler.setMaxContentLength(MAX_TIME_SIZE);
  server->addHandler(&_timeHandler);
#ifdef ESP32
  WiFi.onEvent(
      std::bind(&Ser2netSettingsService::onStationModeDisconnected, this, std::placeholders::_1, std::placeholders::_2),
      WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(std::bind(&Ser2netSettingsService::onStationModeGotIP, this, std::placeholders::_1, std::placeholders::_2),
               WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
#elif defined(ESP8266)
  _onStationModeDisconnectedHandler = WiFi.onStationModeDisconnected(
      std::bind(&Ser2netSettingsService::onStationModeDisconnected, this, std::placeholders::_1));
  _onStationModeGotIPHandler =
      WiFi.onStationModeGotIP(std::bind(&Ser2netSettingsService::onStationModeGotIP, this, std::placeholders::_1));
#endif
  addUpdateHandler([&](const String& originId) { configureSer2net(); }, false);
}

void Ser2netSettingsService::begin() {
  _fsPersistence.readFromFS();
  configureSer2net();
}

#ifdef ESP32
void Ser2netSettingsService::onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println(F("Got IP address, starting ser2net synchronization"));
  configureSer2net();
}

void Ser2netSettingsService::onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println(F("WiFi connection dropped, stopping Ser2net."));
  configureSer2net();
}
#elif defined(ESP8266)
void Ser2netSettingsService::onStationModeGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.println(F("Got IP address, starting Ser2net Synchronization"));
  configureSer2net();
}

void Ser2netSettingsService::onStationModeDisconnected(const WiFiEventStationModeDisconnected& event) {
  Serial.println(F("WiFi connection dropped, stopping Ser2net."));
  configureSer2net();
}
#endif

void Ser2netSettingsService::configureSer2net() {
  if (WiFi.isConnected() && _state.enabled) {
    Serial.println(F("Starting Ser2net..."));
  }
}

void Ser2netSettingsService::configureTime(AsyncWebServerRequest* request, JsonVariant& json) {
  if (json.is<JsonObject>()) {
    String timeUtc = json["time_utc"];
    struct tm tm = {0};
    char* s = strptime(timeUtc.c_str(), "%Y-%m-%dT%H:%M:%SZ", &tm);
    if (s != nullptr) {
      time_t time = mktime(&tm);
      struct timeval now = {.tv_sec = time};
      settimeofday(&now, nullptr);
      AsyncWebServerResponse* response = request->beginResponse(200);
      request->send(response);
      return;
    }
  }
  AsyncWebServerResponse* response = request->beginResponse(400);
  request->send(response);
}
