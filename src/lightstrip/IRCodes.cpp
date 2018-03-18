#include <lightstrip/IRCodes.h>

boolean isColorCode(uint64_t code){
  switch (code){
    case IR_R:
    case IR_G:
    case IR_B:
    case IR_W:
    case IR_B1:
    case IR_B2:
    case IR_B3:
    case IR_B4:
    case IR_B5:
    case IR_B6:
    case IR_B7:
    case IR_B8:
    case IR_B9:
    case IR_B10:
    case IR_B11:
    case IR_B12:
      return true;
    default:
      return false;
  }
}

CRGB getColorForCode(uint64_t code) {
  switch (code){
    case IR_R:
      return CRGB::Red;
    case IR_G:
      return CRGB::Lime;
    case IR_B:
      return CRGB::Blue;
    case IR_W:
      return CRGB::White;
    case IR_B1:
      return CRGB::IndianRed;
    case IR_B2:
      return CRGB::SpringGreen;
    case IR_B3:
      return CRGB::SteelBlue;
    case IR_B4:
      return CRGB::FireBrick;
    case IR_B5:
      return CRGB::DeepSkyBlue;
    case IR_B6:
      return CRGB::Indigo;
    case IR_B7:
      return CRGB::Coral;
    case IR_B8:
      return CRGB::Turquoise;
    case IR_B9:
      return CRGB::Purple;
    case IR_B10:
      return CRGB::Yellow;
    case IR_B11:
      return CRGB::DarkBlue;
    case IR_B12:
      return CRGB::DeepPink;
    default:
      return CRGB::White;
  }
}
