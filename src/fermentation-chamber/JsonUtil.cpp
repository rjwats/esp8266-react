#include <fermentation-chamber/JsonUtil.h>

void updateDeviceAddressFromJson(JsonObject& root, DeviceAddress deviceAddress, String key) {

}

void writeDeviceAddressToJson(JsonObject& root, DeviceAddress deviceAddress, String key) {
  root[key] = deviceAddressAsString(deviceAddress);
}

String deviceAddressAsString(DeviceAddress deviceAddress) {
  char deviceAddressString[17];
  for (int i = 0; i < 8; i++) {
    sprintf(deviceAddressString + (i * 2), "%02X", deviceAddress[i]);
  }
  return deviceAddressString;
}
