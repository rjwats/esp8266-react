#ifndef ArduinoJsonJWT_H
#define ArduinoJsonJWT_H

#include "base64.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#if defined(ESP_PLATFORM)
  #include <mbedtls/md.h>
#else
  #include <bearssl/bearssl_hmac.h>
#endif 

#define JWT_HEADER_SIZE 36
#define JWT_SIG_SIZE 43

class ArduinoJsonJWT {

private:
  String _secret;

  // {"alg": "HS256", "typ": "JWT"}
  const String JWT_HEADER = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9";
  
  String sign(String &value);
  String encode(unsigned char * value, int length);
  String decode(unsigned char * value);

public:
  ArduinoJsonJWT(String secret);

  void setSecret(String secret);
  String buildJWT(JsonObject &payload);
  void parseJWT(String jwt, JsonDocument &jsonDocument);
};


#endif
