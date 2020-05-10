#ifndef JsonSerializer_h
#define JsonSerializer_h

#include <ArduinoJson.h>

template <class T>
using JsonSerializer = void (*)(T& settings, JsonObject& root);

#endif  // end JsonSerializer
