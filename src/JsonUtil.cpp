#include <JsonUtil.h>

CRGB hexStringToColor(String& hexColor, CRGB def) {
  if (hexColor.length() == 7) {
    return CRGB(strtoll(&hexColor[1], nullptr, 16));
  }
  return def;
}

String colorToHexString(CRGB color) {
  char colorString[8];
  sprintf(colorString, "#%02x%02x%02x", color.r, color.g, color.b);
  return colorString;
}

void rgbJsonToColor(JsonObject& root, CRGB& color, String key) {
  if (root.containsKey(key)) {
    JsonObject jsonObject = root[key];
    color.r = jsonObject["r"] | 0;
    color.g = jsonObject["g"] | 0;
    color.b = jsonObject["b"] | 0;
  }
}

void colorToRGBJson(CRGB& color, JsonObject& root, String key) {
  JsonObject jsonObject = root.createNestedObject(key);
  jsonObject["r"] = color.r;
  jsonObject["g"] = color.g;
  jsonObject["b"] = color.b;
}

/*

void updateBooleanArrayFromJson(JsonObject& root, bool booleanArray[], uint16_t maxLen, String key) {
  if (root.containsKey(key) && root[key].is<JsonArray>()) {
    JsonArray jsonArray = root[key];
    for (uint8_t i = 0; i < maxLen; i++) {
      if (i < jsonArray.size() && jsonArray[i].is<bool>()) {
        booleanArray[i] = jsonArray[i];
      } else {
        booleanArray[i] = false;
      }
    }
  }
}

void writeBooleanArrayToJson(JsonObject& root, bool booleanArray[], uint16_t len, String key) {
  JsonArray array = root.createNestedArray(key);
  for (uint8_t i = 0; i < len; i++) {
    array.add(booleanArray[i]);
  }
}

void updateColorFromJson(JsonObject& root, CRGB* color, CRGB def, String key) {
  if (root.containsKey(key) && root[key].is<const char*>()) {
    String colorString = root[key];
    if (colorString.length() == 7) {
      *color = CRGB(strtoll(&colorString[1], NULL, 16));
    }
  } else {
    *color = def;
  }
}



void updateByteFromJson(JsonObject& root, uint8_t* value, uint8_t def, String key) {
  if (root.containsKey(key) && root[key].is<uint8_t>()) {
    *value = (uint8_t)root[key];
  } else {
    *value = def;
  }
}

void writeByteToJson(JsonObject& root, uint8_t* value, String key) {
  root[key] = (uint8_t)*value;
}

void updateBoolFromJson(JsonObject& root, bool* value, bool def, String key) {
  if (root.containsKey(key) && root[key].is<bool>()) {
    *value = (bool)root[key];
  } else {
    *value = def;
  }
}

void writeBoolToJson(JsonObject& root, bool* value, String key) {
  root[key] = (bool)*value;
}

*/