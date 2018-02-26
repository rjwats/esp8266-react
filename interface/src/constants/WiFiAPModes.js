export const WIFI_AP_MODE_ALWAYS = 0;
export const WIFI_AP_MODE_DISCONNECTED = 1;
export const WIFI_AP_NEVER = 2;

export const isAPEnabled = apMode => apMode === WIFI_AP_MODE_ALWAYS || apMode === WIFI_AP_MODE_DISCONNECTED;
