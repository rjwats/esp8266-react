#ifndef COLOR_UTIL_H
#define COLOR_UTIL_H

#include <ArduinoJson.h>
#include <FastLed.h>

String colorToHexString(CRGB color);
CRGB hexStringToColor(String& hexColor, CRGB def);

// void updateColorFromJson(JsonObject& root, CRGB* color, CRGB def, String key = "color");
void colorToRGBJson(CRGB& color, JsonObject& root, String key = "color");
void rgbJsonToColor(JsonObject& root, CRGB& color, String key = "color");

/*
void updateBooleanArrayFromJson(JsonObject& root, bool booleanArray[], uint16_t maxSize, String key);
void writeBooleanArrayToJson(JsonObject& root, bool booleanArray[], uint16_t maxSize, String key);



void updateByteFromJson(JsonObject& root, uint8_t* value, uint8_t def, String key);
void writeByteToJson(JsonObject& root, uint8_t* value, String key);

void updateBoolFromJson(JsonObject& root, bool* value, bool def, String key);
void writeBoolToJson(JsonObject& root, bool* value, String key);
*/

#endif
