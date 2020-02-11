#include <FeaturesService.h>

FeaturesService::FeaturesService(AsyncWebServer* server) {
  server->on(FEATURES_SERVICE_PATH, HTTP_GET, std::bind(&FeaturesService::features, this, std::placeholders::_1));
}

void FeaturesService::features(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_FEATURES_SIZE);
  JsonObject root = response->getRoot();
#if USING(FT_SECURITY)
  root["security"] = true;
#else
  root["security"] = false;
#endif
#if USING(FT_PROJECT)
  root["project"] = true;
#else
  root["project"] = false;
#endif
  response->setLength();
  request->send(response);
}
