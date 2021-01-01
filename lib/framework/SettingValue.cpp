#include <SettingValue.h>

namespace SettingValue {

#ifdef ESP32
const String PLATFORM = "esp32";
#elif defined(ESP8266)
const String PLATFORM = "esp8266";
#endif

/**
 * Obtains the factory set chip id, which is usually a batch indicator.
 */
String getChipId() {
#ifdef ESP32
  return String((unsigned long)ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
  return String(ESP.getChipId(), HEX);
#endif
}

/**
 * Uses the station's MAC address to create a unique id for each device.
 */
String getUniqueId() {
  uint8_t mac[6];
#ifdef ESP32
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
#elif defined(ESP8266)
  wifi_get_macaddr(STATION_IF, mac);
#endif
  char macStr[13] = {0};
  sprintf(macStr, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

String format(String value) {
  value.replace("#{chip_id}", getChipId());
  value.replace("#{unique_id}", getUniqueId());
  value.replace("#{platform}", PLATFORM);
  return value;
}
};  // end namespace SettingValue
