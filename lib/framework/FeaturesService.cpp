#include <FeaturesService.h>

FeaturesService::FeaturesService(AsyncWebServer* server) {
  server->on(FEATURES_SERVICE_PATH, HTTP_GET, [this](AsyncWebServerRequest* request) {
    this->features(request);
  });
}

void FeaturesService::features(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false);
  JsonObject root = response->getRoot();
#if FT_ENABLED(FT_PROJECT)
  root["project"] = true;
#else
  root["project"] = false;
#endif
#if FT_ENABLED(FT_SECURITY)
  root["security"] = true;
#else
  root["security"] = false;
#endif
#if FT_ENABLED(FT_MQTT)
  root["mqtt"] = true;
#else
  root["mqtt"] = false;
#endif
#if FT_ENABLED(FT_NTP)
  root["ntp"] = true;
#else
  root["ntp"] = false;
#endif
#if FT_ENABLED(FT_OTA)
  root["ota"] = true;
#else
  root["ota"] = false;
#endif
#if FT_ENABLED(FT_UPLOAD_FIRMWARE)
  root["upload_firmware"] = true;
#else
  root["upload_firmware"] = false;
#endif
  response->setLength();
  request->send(response);
}
