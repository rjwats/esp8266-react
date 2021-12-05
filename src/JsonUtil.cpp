#include <JsonUtil.h>

void updateBooleanArrayFromJson(JsonObject& root, bool booleanArray[], uint16_t len, bool def, String key) {
  for (uint8_t i = 0; i < len; i++) {
    booleanArray[i] = def;
  }
  if (root.containsKey(key) && root[key].is<JsonArray>()) {
    JsonArray jsonArray = root[key];
    for (uint8_t i = 0; i < len && i < jsonArray.size(); i++) {
      if (jsonArray[i].is<bool>()) {
        booleanArray[i] = jsonArray[i];
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

void updatePaletteFromJson(JsonObject& root, CRGBPalette16* palette, CRGB def, String key) {
  for (uint8_t i = 0; i < 16; i++) {
    palette->entries[i] = def;
  }
  if (root.containsKey(key) && root[key].is<JsonArray>()) {
    JsonArray jsonArray = root[key];
    for (uint8_t i = 0; i < 16 && i < jsonArray.size(); i++) {
      if (jsonArray[i].is<String>()) {
        String colorString = jsonArray[i];
        if (colorString.length() == 7) {
          palette->entries[i] = CRGB(strtoll(&colorString[1], NULL, 16));
        }
      }
    }
  }
}

void writePaletteToJson(JsonObject& root, const CRGBPalette16* palette, String key) {
  JsonArray array = root.createNestedArray(key);
  for (uint8_t i = 0; i < 16; i++) {
    CRGB color = palette->entries[i];
    char colorString[8];
    sprintf(colorString, "#%02x%02x%02x", color.r, color.g, color.b);
    array.add(colorString);
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

void writeColorToJson(JsonObject& root, CRGB* color, String key) {
  char colorString[8];
  sprintf(colorString, "#%02x%02x%02x", color->r, color->g, color->b);
  root[key] = colorString;
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
