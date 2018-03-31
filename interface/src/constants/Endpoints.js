export const ENDPOINT_ROOT = "http://192.168.0.20";
export const WS_ROOT = "ws://192.168.0.20";

export const NTP_STATUS_PATH = "/ntpStatus";
export const NTP_STATUS_ENDPOINT = ENDPOINT_ROOT + NTP_STATUS_PATH;

export const NTP_SETTINGS_PATH = "/ntpSettings";
export const NTP_SETTINGS_ENDPOINT = ENDPOINT_ROOT + NTP_SETTINGS_PATH;

export const AP_SETTINGS_PATH = "/apSettings";
export const AP_SETTINGS_ENDPOINT = ENDPOINT_ROOT + AP_SETTINGS_PATH;

export const AP_STATUS_PATH = "/apStatus";
export const AP_STATUS_ENDPOINT = ENDPOINT_ROOT + AP_STATUS_PATH;

export const SCAN_NETWORKS_PATH = "/scanNetworks";
export const SCAN_NETWORKS_ENDPOINT = ENDPOINT_ROOT + SCAN_NETWORKS_PATH;

export const LIST_NETWORKS_PATH = "/listNetworks";
export const LIST_NETWORKS_ENDPOINT = ENDPOINT_ROOT + LIST_NETWORKS_PATH;

export const WIFI_SETTINGS_PATH = "/wifiSettings";
export const WIFI_SETTINGS_ENDPOINT = ENDPOINT_ROOT + WIFI_SETTINGS_PATH;

export const WIFI_STATUS_PATH = "/wifiStatus";
export const WIFI_STATUS_ENDPOINT = ENDPOINT_ROOT + WIFI_STATUS_PATH;

export const OTA_SETTINGS_PATH = "/otaSettings";
export const OTA_SETTINGS_ENDPOINT = ENDPOINT_ROOT + OTA_SETTINGS_PATH;

export const LIGHT_STRIP_WEBSOCKET_PATH = "/lightStripWS";
export const LIGHT_STRIP_WEBSOCKET_URI = calculateWebSocketPath(LIGHT_STRIP_WEBSOCKET_PATH);

function calculateWebSocketPath(webSocketPath) {
  if (WS_ROOT.length === 0){
    var loc = window.location, webSocketURI;
    if (loc.protocol === "https:") {
        webSocketURI = "wss:";
    } else {
        webSocketURI = "ws:";
    }
    return webSocketURI + "//" + loc.host + webSocketPath;
  }
  return WS_ROOT + webSocketPath;
}
