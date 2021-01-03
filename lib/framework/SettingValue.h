#ifndef SettingValue_h
#define SettingValue_h

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

namespace SettingValue {
String format(String value);
};

#endif  // end SettingValue
