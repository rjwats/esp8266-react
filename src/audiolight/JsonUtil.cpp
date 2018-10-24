#include <audiolight/JsonUtil.h>

void updateBooleanArrayFromJson(JsonObject& root, bool booleanArray[], uint16_t maxLen, String key){
  if (root.containsKey(key) && root[key].is<JsonArray>()){
    JsonArray& jsonArray = root.get<JsonArray>(key);
    for (uint8_t i = 0; i < maxLen && i < jsonArray.size(); i++) {   
      if (jsonArray.is<bool>(i)) {
        booleanArray[i] = jsonArray[i];
      }
    }
  }
}

void writeBooleanArrayToJson(JsonObject& root, bool booleanArray[], uint16_t len, String key){
  JsonArray& array = root.createNestedArray(key);
  for (uint8_t i = 0; i < len; i++) {
    array.add(booleanArray[i]);
  }
}

void updateColorFromJson(JsonObject& root, CRGB* color, String key){
  if (root.containsKey(key) && root[key].is<const char*>()){
    String colorString =  root[key];
    if (colorString.length() == 7){
      *color = CRGB(strtoll(&colorString[1], NULL, 16));
    }
  }
}

void writeColorToJson(JsonObject& root, CRGB* color, String key){
  char colorString[8];
  sprintf(colorString,"#%02x%02x%02x", color->r, color->g, color->b);
  root[key] = colorString;
}

void updateByteFromJson(JsonObject& root, uint8_t* value, String key){
  if (root.containsKey(key) && root[key].is<uint8_t>()){
    *value = (uint8_t) root[key];
  }
}

void writeByteToJson(JsonObject& root, uint8_t* value, String key){
  root[key] = (uint8_t) *value;
}
