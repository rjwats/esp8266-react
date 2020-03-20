#ifndef SettingsSerializer_h
#define SettingsSerializer_h

#include <ArduinoJson.h>

template <class T>
class SettingsSerializer {
 public:
  virtual void serialize(T& settings, JsonObject root) = 0;
};

#endif  // end SettingsSerializer
