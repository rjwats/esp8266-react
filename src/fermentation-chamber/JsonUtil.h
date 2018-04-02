#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include <DallasTemperature.h>
#include <ArduinoJson.h>

void updateDeviceAddressFromJson(JsonObject& root, DeviceAddress deviceAddress, String key);
void writeDeviceAddressToJson(JsonObject& root, DeviceAddress deviceAddress, String key);
String deviceAddressAsString(DeviceAddress deviceAddress);

#endif
