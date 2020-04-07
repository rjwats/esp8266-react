#ifndef SettingsDeserializer_h
#define SettingsDeserializer_h

#include <ArduinoJson.h>

template <class T>
class SettingsDeserializer {
 public:
  virtual void deserialize(T& settings, JsonObject root) = 0;
};

#endif  // end SettingsDeserializer
