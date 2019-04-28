#include <ESPStatus.h>

ESPStatus::ESPStatus(AsyncWebServer *server) : _server(server) {
  _server->on(ESP_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&ESPStatus::espStatus, this, std::placeholders::_1));
}

void ESPStatus::espStatus(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse(MAX_ESP_STATUS_SIZE);
  JsonObject root = response->getRoot();

#if defined(ESP8266) 
  root["esp_platform"] = "esp8266";
  
  // metrics from ESP (https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#esp-specific-apis)
  root["cycle_count"] = ESP.getCycleCount();
  root["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
  root["last_reset_reason"] = ESP.getResetReason();
  //Internal RAM
  root["free_heap"] = ESP.getFreeHeap();
  root["heap_fragmentation"] = ESP.getHeapFragmentation();
  root["max_free_block_size"] = ESP.getMaxFreeBlockSize();

  root["chip_id"] = this->getChipId();
  root["core_version"] = ESP.getCoreVersion();
  root["sdk_version"] = ESP.getSdkVersion();
  //Sketch
  root["sketch_size"] = ESP.getSketchSize();
  root["free_sketch_space"] = ESP.getFreeSketchSpace();
  root["sketch_md5"] = ESP.getSketchMD5();
  //Flash
  root["flash_chip_id"] = ESP.getFlashChipId();
  root["flash_chip_size"] = ESP.getFlashChipSize();
  root["flash_chip_real_size"] = ESP.getFlashChipRealSize();
  root["flash_chip_speed"] = ESP.getFlashChipSpeed();
  //MAC
  root["current_mac_address"] = WiFi.macAddress();

#elif defined(ESP_PLATFORM)
  root["esp_platform"] = "esp32";

  root["cycle_count"] = ESP.getCycleCount();
  root["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
  //Internal RAM
  root["heap_size"] = ESP.getHeapSize();
  root["free_heap"] = ESP.getFreeHeap();
  root["min_free_heap"] = ESP.getMinFreeHeap();
  root["max_alloc_heap"] = ESP.getMaxAllocHeap();
  //SPI RAM
  root["psram_size"] = ESP.getPsramSize();
  root["free_psram"] = ESP.getFreePsram();
  root["min_free_psram"] = ESP.getMinFreePsram();
  root["max_alloc_psram"] = ESP.getMaxAllocPsram();
  //Sketch
  root["sketch_size"] = ESP.getSketchSize();
  root["sketch_md5"] = ESP.getSketchMD5();
  root["free_sketch_space"] = ESP.getFreeSketchSpace();

  root["chip_revision"] = ESP.getChipRevision();
  root["sdk_version"] = ESP.getSdkVersion();
  //Flash
  root["flash_chip_size"] = ESP.getFlashChipSize();
  root["flash_chip_speed"] = ESP.getFlashChipSpeed();
  root["flash_chip_mode"] = ESP.getFlashChipMode();
  //MAC Address
  root["factory_mac_address"] = this->getEfuseMac();
  root["current_mac_address"] = WiFi.macAddress();
#endif

  response->setLength();
  request->send(response);
}

#if defined(ESP8266) 
String ESPStatus::getChipId() {
  const uint32_t orig_id = ESP.getChipId();
  const uint8_t* id = (uint8_t*)&orig_id;
  char macStr[12] = { 0 };
  sprintf(macStr, "%02X:%02X:%02X:%02X", id[3], id[2], id[1], id[0]);
  return String(macStr);
}
#elif defined(ESP_PLATFORM)
String ESPStatus::getEfuseMac() {
  const uint64_t orig_mac = ESP.getEfuseMac();
  const uint8_t* mac = (uint8_t*)&orig_mac;
  char macStr[18] = { 0 };
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}
#endif
