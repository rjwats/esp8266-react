#include <audiolight/JsonUtil.h>

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
