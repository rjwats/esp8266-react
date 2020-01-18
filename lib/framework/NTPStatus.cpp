#include <NTPStatus.h>

NTPStatus::NTPStatus(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(NTP_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&NTPStatus::ntpStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
}

void NTPStatus::ntpStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_NTP_STATUS_SIZE);
  JsonObject root = response->getRoot();

  // request time now first, this can sometimes force a sync
  root["now"] = time(nullptr);

  // root["status"] = (int)status;
  // root["last_sync"] = lastSync;
  // root["server"] = NTP.getNtpServerName();
  // root["interval"] = NTP.getInterval();
  // root["uptime"] = NTP.getUptime();

  // only add now to response if we have successfully synced
  // if (status != timeNotSet) {
  //  timeNow;
  //}

  response->setLength();
  request->send(response);
}
