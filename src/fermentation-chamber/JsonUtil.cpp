#include <fermentation-chamber/JsonUtil.h>

void deviceAddressFromString(String deviceAddressString, DeviceAddress deviceAddress) {
   if (deviceAddressString.length() == 16) {
     uint64_t deviceAddressAsLong = strtol(deviceAddressString.c_str(), NULL, 16);
     for (int i = 0 ; i < 8 ; i++) {
         deviceAddress[i] = deviceAddressAsLong >> (8 * i);
     }
   }
}

 String deviceAddressAsString(DeviceAddress deviceAddress) {
  uint64_t deviceAddressAsLong = 0;
  for (int i = 0 ; i < 8; i++) {
      deviceAddressAsLong |= deviceAddress[i] << (8 * i);
  }
  char buffer[17];
	ltoa(deviceAddressAsLong, buffer, 16);
  return buffer;
}
