#ifndef SettingsDeserializer_h
#define SettingsDeserializer_h

#include <ArduinoJson.h>

template <class T>
using SettingsDeserializer = void (*)(JsonObject& root, T& settings);


#endif  // end SettingsDeserializer
