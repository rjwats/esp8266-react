#ifdef ESP32
#include <FS.h>
#define DeviceFS SPIFFS
#elif defined(ESP8266)
#include <LittleFS.h>
#define DeviceFS LittleFS
#endif
