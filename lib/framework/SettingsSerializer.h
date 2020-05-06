#ifndef SettingsSerializer_h
#define SettingsSerializer_h

#include <ArduinoJson.h>

template <class T>
using SettingsSerializer = void (*)(T& settings, JsonObject& root);

#endif  // end SettingsSerializer
