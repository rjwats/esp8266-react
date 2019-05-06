#include "ArduinoJsonJWT.h"

ArduinoJsonJWT::ArduinoJsonJWT(String secret) : _secret(secret) { }

void ArduinoJsonJWT::setSecret(String secret){
    _secret = secret;
}

/*
 * ESP32 uses mbedtls, ESP2866 uses bearssl.
 * 
 * Both come with decent HMAC implmentations supporting sha256, as well as others.
 * 
 * No need to pull in additional crypto libraries - lets use what we already have.
 */
String ArduinoJsonJWT::sign(String &payload) {
  unsigned char hmacResult[32];
  {
  #if defined(ESP_PLATFORM)
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;  
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
    mbedtls_md_hmac_starts(&ctx, (unsigned char *) _secret.c_str(), _secret.length());
    mbedtls_md_hmac_update(&ctx, (unsigned char *) payload.c_str(), payload.length());
    mbedtls_md_hmac_finish(&ctx, hmacResult);
    mbedtls_md_free(&ctx);
  #else
    br_hmac_key_context keyCtx;
    br_hmac_key_init(&keyCtx, &br_sha256_vtable, _secret.c_str(), _secret.length());
    br_hmac_context hmacCtx;
    br_hmac_init(&hmacCtx, &keyCtx, 0);
    br_hmac_update(&hmacCtx, payload.c_str(), payload.length());
    br_hmac_out(&hmacCtx, hmacResult);
  #endif
  }
  return encode(hmacResult, 32);
}

String ArduinoJsonJWT::decode(unsigned char * value) {
  // create buffer of approperate length
  size_t decodedLength = decode_base64_length(value) + 1;
  char decoded[decodedLength];

  // decode
  decode_base64(value, (unsigned char *) decoded);
  decoded[decodedLength-1] = 0;

  // return as arduino string
  return String(decoded);
}

String ArduinoJsonJWT::encode(unsigned char * value , int length) {
  int encodedIndex = encode_base64_length(length);

  // encode
  char encoded[encodedIndex];
  encode_base64(value, length, (unsigned char *) encoded);

  // trim padding
  while (encoded[--encodedIndex] == '=') {
    encoded[encodedIndex] = 0;
  }  

  // return as string
  return String(encoded);
}

String ArduinoJsonJWT::buildJWT(JsonObject &payload) {
  // serialize, then encode payload
  String jwt;
  serializeJson(payload, jwt);
  jwt = encode((unsigned char *) jwt.c_str(), jwt.length());

  // add the header to payload
  jwt = JWT_HEADER + '.' + jwt;
  
  // add signature
  jwt  += '.' + sign(jwt);

  return jwt;
}

void ArduinoJsonJWT::parseJWT(String jwt, JsonDocument &jsonDocument) {
  // clear json document before we begin, jsonDocument wil be null on failure
  jsonDocument.clear();

  // must be of minimum length or greater
  if (jwt.length() <= JWT_SIG_SIZE + JWT_HEADER_SIZE + 2) {
    return;
  }
  // must have the correct header and delimiter
  if (!jwt.startsWith(JWT_HEADER) || jwt.indexOf('.') != JWT_HEADER_SIZE) {
    return;
  }
  // must have signature of correct length
  int signatureDelimieterIndex = jwt.length() - JWT_SIG_SIZE - 1;
  if (jwt.lastIndexOf('.') != signatureDelimieterIndex) {
    return;
  }

  // signature must be correct
  String signature = jwt.substring(signatureDelimieterIndex + 1);
  jwt = jwt.substring(0, signatureDelimieterIndex);
  if (sign(jwt) != signature){
    return;
  }

  // decode payload
  jwt = jwt.substring(JWT_HEADER_SIZE + 1); 
  jwt = decode((unsigned char *) jwt.c_str());
  
  // parse payload, clearing json document after failure
  DeserializationError error = deserializeJson(jsonDocument, jwt);
  if (error != DeserializationError::Ok || !jsonDocument.is<JsonObject>()){
    jsonDocument.clear();
  }
}
