#ifndef ArduinoJsonJWT_H
#define ArduinoJsonJWT_H

#include "jwt/base64.h"
#include "jwt/sha256.h"
#include "jwt/ArduinoJsonJWT.h"

#include <Arduino.h>
#include <ArduinoJson.h>

class ArduinoJsonJWT {

private:
  String _psk;

  // {"alg": "HS256", "typ": "JWT"}
  const char* JWT_HEADER = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9";
  const uint16_t JWT_HEADER_LENGTH = strlen(JWT_HEADER);
  const uint16_t SIGNATURE_LENGTH = encode_base64_length(32);  

  // static JWT length is made of:
  // - the header length
  // - the signature length
  // - 2 delimiters, 1 terminator
  const uint16_t BASE_JWT_LENGTH = JWT_HEADER_LENGTH + SIGNATURE_LENGTH + 3;

public:
  ArduinoJsonJWT(String psk);
  void setPSK(String psk);
  String encodeJWT(JsonObject payload);
};


#endif
