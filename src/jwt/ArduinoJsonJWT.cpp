#include "jwt/ArduinoJsonJWT.h"

ArduinoJsonJWT::ArduinoJsonJWT(String psk) : _psk(psk) { }

void ArduinoJsonJWT::setPSK(String psk){
    _psk = psk;
}

String ArduinoJsonJWT::encodeJWT(JsonObject payload) {
  // serialize payload
  String serializedPayload;
  serializeJson(payload, serializedPayload);

  // calculate length of string
  uint16_t encodedPayloadLength = encode_base64_length(serializedPayload.length());

  // create JWT char array
  char encodedJWT[BASE_JWT_LENGTH + encodedPayloadLength];
  unsigned char* ptr = (unsigned char*) encodedJWT;

  // 1 - add the header
  memcpy(ptr, JWT_HEADER, JWT_HEADER_LENGTH);
  ptr += JWT_HEADER_LENGTH;

  // 2 - add payload, trim and null terminate
  *ptr++ = '.';
  encode_base64((unsigned char*) serializedPayload.c_str(), serializedPayload.length(), ptr);
  ptr += encodedPayloadLength;
  while(*(ptr - 1) == '=') {
    ptr--;
  }
  *(ptr) = 0;
  
  // ... calculate ...
  Sha256.initHmac((const unsigned char*)_psk.c_str(), _psk.length());
  Sha256.print(encodedJWT);

  // 3 - add signature
  *ptr++ = '.';
  encode_base64(Sha256.resultHmac(), 32, ptr);
  ptr += SIGNATURE_LENGTH;
  while(*(ptr - 1) == '=') {
    ptr--;
  }
  *(ptr) = 0;

  Serial.println(BASE_JWT_LENGTH + encodedPayloadLength);
  return encodedJWT;
}

