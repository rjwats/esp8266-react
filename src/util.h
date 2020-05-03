#ifndef util_h
#define util_h

#include <Arduino.h>

static String defaultDeviceValue(String prefix = "") {
#ifdef ESP32
  return prefix + String((unsigned long)ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
  return prefix + String(ESP.getChipId(), HEX);
#endif
}

#endif
