#include <ESPStatus.h>

ESPStatus::ESPStatus(AsyncWebServer *server) : _server(server) {
  _server->on(ESP_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&ESPStatus::espStatus, this, std::placeholders::_1));
}

void ESPStatus::espStatus(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse(MAX_ESP_STATUS_SIZE);
  JsonObject root = response->getRoot();

#if defined(ESP8266) 
  // metric from ESP (https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#esp-specific-apis)
  root["last_reset_reason"] = ESP.getResetReason();
  root["free_heap"] = ESP.getFreeHeap();
  root["heap_fragmentation"] = ESP.getHeapFragmentation();
  root["max_free_block_size"] = ESP.getMaxFreeBlockSize();
  root["chip_id"] = ESP.getChipId();
  root["core_version"] = ESP.getCoreVersion();
  root["sdk_version"] = ESP.getSdkVersion();
  root["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
  root["sketch_size"] = ESP.getSketchSize();
  root["free_sketch_space"] = ESP.getFreeSketchSpace();
  root["sketch_md5"] = ESP.getSketchMD5();
  root["flash_chip_id"] = ESP.getFlashChipId();
  root["flash_chip_size"] = ESP.getFlashChipSize();
  root["flash_chip_real_size"] = ESP.getFlashChipRealSize();
  root["flash_chip_speed"] = ESP.getFlashChipSpeed();
  root["cycle_count"] = ESP.getCycleCount();
#endif

  response->setLength();
  request->send(response);
}
