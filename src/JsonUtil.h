#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include <DallasTemperature.h>
#include <ArduinoJson.h>

String deviceAddressAsString(DeviceAddress deviceAddress);
void deviceAddressFromString(String deviceAddressString, DeviceAddress deviceAddress);

#endif
