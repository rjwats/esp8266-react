#include <SettingValue.h>

namespace SettingValue {
String format(String value) {
#ifdef ESP32
  value.replace("#{chip_id}", String((unsigned long)ESP.getEfuseMac(), HEX));
  value.replace("#{platform}", "esp32");
#elif defined(ESP8266)
  value.replace("#{chip_id}", String(ESP.getChipId(), HEX));
  value.replace("#{platform}", "esp8266");
#endif

  return value;
}
};  // end namespace SettingValue
