#ifndef IR_CODES_H
#define IR_CODES_H

#include <FastLED.h>

#define	IR_BPLUS  0xF700FF
#define	IR_BMINUS 0xF7807F
#define	IR_OFF 	  0xF740BF
#define	IR_ON 	  0xF7C03F
#define	IR_R 	  0xF720DF
#define	IR_G 	  0xF7A05F
#define	IR_B 	  0xF7609F
#define	IR_W 	  0xF7E01F
#define	IR_B1	  0xF710EF
#define	IR_B2	  0xF7906F
#define	IR_B3	  0xF750AF
#define	IR_FLASH  0xF7D02F
#define	IR_B4	  0xF730CF
#define	IR_B5	  0xF7B04F
#define	IR_B6	  0xF7708F
#define	IR_STROBE 0xF7F00F
#define	IR_B7	  0xF708F7
#define	IR_B8	  0xF78877
#define	IR_B9	  0xF748B7
#define	IR_FADE   0xF7C837
#define	IR_B10	  0xF728D7
#define	IR_B11	  0xF7A857
#define	IR_B12	  0xF76897
#define	IR_SMOOTH 0xF7E817
#define	IR_REPEAT	  0xFFFFFFFFFFFFFFFF

boolean isColorCode(uint64_t code);
CRGB getColorForCode(uint64_t code);

#endif
