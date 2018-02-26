export const WIFI_SECURITY_WPA_TKIP = 2;
export const WIFI_SECURITY_WEP = 5;
export const WIFI_SECURITY_WPA_CCMP = 4;
export const WIFI_SECURITY_NONE = 7;
export const WIFI_SECURITY_AUTO = 8;

export const isNetworkOpen = selectedNetwork => selectedNetwork && selectedNetwork.encryption_type === WIFI_SECURITY_NONE;

export const networkSecurityMode = selectedNetwork => {
  switch (selectedNetwork.encryption_type){
    case WIFI_SECURITY_WPA_TKIP:
    case WIFI_SECURITY_WPA_CCMP:
      return "WPA";
    case WIFI_SECURITY_WEP:
      return "WEP";
    case WIFI_SECURITY_AUTO:
      return "Auto";
    case WIFI_SECURITY_NONE:
      return "None";
    default:
      return "Unknown";
  }
}
