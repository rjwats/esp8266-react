#include <fermentation-chamber/JsonUtil.h>

const DeviceAddress EMPTY_ADDRESS =  {0, 0, 0, 0, 0, 0, 0, 0};

void setNullDeviceAddress(DeviceAddress deviceAddress) {
   for (int i = 0; i < 8; i++) {
     deviceAddress[i] = 0;
   }
}

void deviceAddressFromString(String deviceAddressString, DeviceAddress deviceAddress) {
   if (deviceAddressString.length() == 16){
     for (int i = 0; i < 8; i++) {
       if (!sscanf(deviceAddressString.c_str() + 2*i, "%02x", &deviceAddress[i])) {
         setNullDeviceAddress(deviceAddress);
         return;
       }
     }
     return;
   }
   setNullDeviceAddress(deviceAddress);
}

String deviceAddressAsString(DeviceAddress deviceAddress) {
  char deviceAddressString[17];
  for (int i = 0; i < 8; i++) {
    sprintf(deviceAddressString + (i * 2), "%02X", deviceAddress[i]);
  }
  return deviceAddressString;
}
