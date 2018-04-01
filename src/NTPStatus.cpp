#include <NTPStatus.h>

NTPStatus::NTPStatus(AsyncWebServer *server) : _server(server) {
  _server->on(NTP_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&NTPStatus::ntpStatus, this, std::placeholders::_1));
}

void NTPStatus::ntpStatus(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  JsonObject& root = response->getRoot();

  // request time now first, this can sometimes force a sync
  time_t timeNow = now();
  timeStatus_t  status = timeStatus();
  time_t lastSync = NTP.getLastNTPSync();
  root["status"] = (int) status;
  root["last_sync"] = lastSync;
  root["server"] = NTP.getNtpServerName();
  root["interval"] = NTP.getInterval();
  root["uptime"] = NTP.getUptime();

  // only add now to response if we have successfully synced
  if (status != timeNotSet){
    root["now"] = timeNow;
  }

  response->setLength();
  request->send(response);
}
