#ifndef JsonDeserializer_h
#define JsonDeserializer_h

#include <ArduinoJson.h>

template <class T>
using JsonDeserializer = void (*)(JsonObject& root, T& settings);

#endif  // end JsonDeserializer
