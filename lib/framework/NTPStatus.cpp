#include <NTPStatus.h>

NTPStatus::NTPStatus(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(NTP_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&NTPStatus::ntpStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
}

/*
 * Formats the time using the format provided.
 *
 * Uses a 25 byte buffer, large enough to fit an ISO time string with offset.
 */
String formatTime(tm* time, const char* format) {
  char time_string[25];
  strftime(time_string, 25, format, time);
  return String(time_string);
}

String toUTCTimeString(tm* time) {
  return formatTime(time, "%FT%TZ");
}

String toLocalTimeString(tm* time) {
  return formatTime(time, "%FT%T");
}

String offsetString(tm* time) {
  return formatTime(time, "%z");
}

void NTPStatus::ntpStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_NTP_STATUS_SIZE);
  JsonObject root = response->getRoot();

  // grab the current instant in unix seconds
  time_t now = time(nullptr);

  // only provide enabled/disabled status for now
  root["status"] = sntp_enabled() ? 1 : 0;

  // the current time in UTC
  root["time_utc"] = toUTCTimeString(gmtime(&now));

  // local time with offset separate
  struct tm* ltm = localtime(&now);
  root["time_local"] = toLocalTimeString(ltm);
  root["time_offset"] = offsetString(ltm);

  // the sntp server name
  root["server"] = sntp_getservername(0);

  // device uptime in seconds
  root["uptime"] = millis() / 1000;

  response->setLength();
  request->send(response);
}
