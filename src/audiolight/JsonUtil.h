#ifndef COLOR_UTIL_H
#define COLOR_UTIL_H

#include <ArduinoJson.h>
#include <FastLed.h>

void updateColorFromJson(JsonObject& root, CRGB* color, String key = "color");
void writeColorToJson(JsonObject& root, CRGB* color, String key = "color");

void updateByteFromJson(JsonObject& root, uint8_t* value, String key);
void writeByteToJson(JsonObject& root, uint8_t* value, String key);

#endif
