#include <SettingValue.h>

namespace SettingValue {
String format(String value) {
#ifdef ESP32
  String chipId((unsigned long)ESP.getEfuseMac(), HEX);
  String platform(F("esp32"));
#elif defined(ESP8266)
  String chipId(ESP.getChipId(), HEX);
  String platform(F("esp8266"));
#endif
  value.replace(F("#{chip_id}"), chipId);
  value.replace(F("#{platform}"), platform);
  return value;
}
};  // end namespace SettingValue
