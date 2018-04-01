const ENDPOINT_HOST = "http://192.168.0.20";
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

const WEB_SOCKET_HOST = "ws://192.168.0.20";
const WEB_SOCKET_PATH = "/ws";
const WEB_SOCKET_PREFIX = calculateWebSocketPrefix(WEB_SOCKET_PATH);

export const LIGHT_STRIP_WEBSOCKET_URI = WEB_SOCKET_PREFIX + "/lightStrip";

function calculateWebSocketPrefix(webSocketPath) {
  if (WEB_SOCKET_HOST.length === 0){
    var loc = window.location, webSocketURI;
    if (loc.protocol === "https:") {
        webSocketURI = "wss:";
    } else {
        webSocketURI = "ws:";
    }
    return webSocketURI + "//" + loc.host + webSocketPath;
  }
  return WEB_SOCKET_HOST + webSocketPath;
}
