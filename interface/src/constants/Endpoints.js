const ENDPOINT_HOST = "http://192.168.0.19";
const ENDPOINT_PATH = "/rest";
const ENDPOINT_PREFIX = ENDPOINT_HOST + ENDPOINT_PATH;

export const NTP_STATUS_ENDPOINT = ENDPOINT_PREFIX + "/ntpStatus";
export const NTP_SETTINGS_ENDPOINT = ENDPOINT_PREFIX + "/ntpSettings";
export const AP_SETTINGS_ENDPOINT = ENDPOINT_PREFIX + "/apSettings";
export const AP_STATUS_ENDPOINT = ENDPOINT_PREFIX + "/apStatus";
export const SCAN_NETWORKS_ENDPOINT = ENDPOINT_PREFIX + "/scanNetworks";
export const LIST_NETWORKS_ENDPOINT = ENDPOINT_PREFIX + "/listNetworks";
export const WIFI_SETTINGS_ENDPOINT = ENDPOINT_PREFIX + "/wifiSettings";
export const WIFI_STATUS_ENDPOINT = ENDPOINT_PREFIX + "/wifiStatus";
export const OTA_SETTINGS_ENDPOINT = ENDPOINT_PREFIX + "/otaSettings";
export const CHAMBER_SETTINGS_ENDPOINT = ENDPOINT_PREFIX + "/chamberSettings";
export const CHAMBER_STATUS_ENDPOINT = ENDPOINT_PREFIX + "/chamberStatus";
