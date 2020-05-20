#ifndef JsonUtils_h
#define JsonUtils_h

#include <Arduino.h>
#include <IPAddress.h>
#include <ArduinoJson.h>

class JsonUtils {
 public:
  static void readIP(JsonObject& root, String key, IPAddress& _ip) {
    if (!root[key].is<String>() || !_ip.fromString(root[key].as<String>())) {
      _ip = INADDR_NONE;
    }
  }
  static void writeIP(JsonObject& root, String key, IPAddress& _ip) {
    if (_ip != INADDR_NONE) {
      root[key] = _ip.toString();
    }
  }
};

#endif  // end JsonUtils
