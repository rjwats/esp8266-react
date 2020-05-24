#include <SystemStatus.h>
#ifdef ESP32
#include <esp_spiffs.h>
#include <SPIFFS.h>
#elif defined(ESP8266)
#include <FS.h>
#endif

SystemStatus::SystemStatus(AsyncWebServer* server, SecurityManager* securityManager) {
  server->on(SYSTEM_STATUS_SERVICE_PATH,
             HTTP_GET,
             securityManager->wrapRequest(std::bind(&SystemStatus::systemStatus, this, std::placeholders::_1),
                                          AuthenticationPredicates::IS_AUTHENTICATED));
}

void SystemStatus::systemStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_ESP_STATUS_SIZE);
  JsonObject root = response->getRoot();
#ifdef ESP32
  root["esp_platform"] = "esp32";
  root["max_alloc_heap"] = ESP.getMaxAllocHeap();
#elif defined(ESP8266)
  root["esp_platform"] = "esp8266";
  root["max_alloc_heap"] = ESP.getMaxFreeBlockSize();
#endif
  root["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
  root["free_heap"] = ESP.getFreeHeap();
  root["sketch_size"] = ESP.getSketchSize();
  root["free_sketch_space"] = ESP.getFreeSketchSpace();
  root["sdk_version"] = ESP.getSdkVersion();
  root["flash_chip_size"] = ESP.getFlashChipSize();
  root["flash_chip_speed"] = ESP.getFlashChipSpeed();
#ifdef ESP32 
  if (esp_spiffs_mounted(NULL)) {
    root["spiffs_used"] = SPIFFS.usedBytes();
    root["spiffs_size"] = SPIFFS.totalBytes();
  }
#elif !defined(PROGMEM_WWW) //couldn't find an esp8266 alternative to esp_spiffs_mounted()
    root["spiffs_used"] = SPIFFS.usedBytes();
    root["spiffs_size"] = SPIFFS.totalBytes();
#endif  
  response->setLength();
  request->send(response);
}
