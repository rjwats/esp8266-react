#include <ESPUtils.h>

namespace FactoryValue {
  String format(String tpl) {
#ifdef ESP32
    String chipId((unsigned long)ESP.getEfuseMac(), HEX);
    String patform(F("esp32"));
#elif defined(ESP8266)
    String chipId(ESP.getChipId(), HEX);
    String patform(F("esp8266"));
#endif

    tpl.replace(F("${chip_id}"), chipId);
    tpl.replace(F("${platform}"), patform);
    return tpl;
  }
};