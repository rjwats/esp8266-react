#include <JsonUtil.h>

void deviceAddressFromString(String deviceAddressString, DeviceAddress deviceAddress) {
   uint64_t deviceAddressAsLong = 0;
   if (deviceAddressString.length() == 16){
      deviceAddressAsLong = strtoull(deviceAddressString.c_str(), NULL, 16);
   }
   for (uint8_t i = 0; i < 8; i++) {
     deviceAddress[i] = deviceAddressAsLong >> (8 * i);
   }
}

String deviceAddressAsString(DeviceAddress deviceAddress) {
  char deviceAddressString[17];
  for (uint8_t i = 0; i < 8; i++) {
    sprintf(deviceAddressString + (i * 2), "%02x", deviceAddress[7 - i]);
  }
  return deviceAddressString;
}
