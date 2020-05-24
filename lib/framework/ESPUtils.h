#ifndef ESPUtils_h
#define ESPUtils_h

#include <Arduino.h>

class ESPUtils {
 public:
  static String defaultDeviceValue(String prefix = "") {
#ifdef ESP32
    return prefix + String((unsigned long)ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
    return prefix + String(ESP.getChipId(), HEX);
#endif
  }

  static String toISOString(tm* time, bool incOffset) {
    char time_string[25];
    strftime(time_string, 25, incOffset ? "%FT%T%z" : "%FT%TZ", time);
    return String(time_string);
  }

  static String toHrString(tm* time) {
    char time_string[22];
    strftime(time_string, 22, "%F %T%z", time);
    return time_string;
  }
};

#endif  // end ESPUtils
