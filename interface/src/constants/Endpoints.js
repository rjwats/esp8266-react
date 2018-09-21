const ENDPOINT_ROOT = process.env.REACT_APP_ENDPOINT_ROOT;

export const NTP_STATUS_ENDPOINT = ENDPOINT_ROOT + "ntpStatus";
export const NTP_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "ntpSettings";
export const AP_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "apSettings";
export const AP_STATUS_ENDPOINT = ENDPOINT_ROOT + "apStatus";
export const SCAN_NETWORKS_ENDPOINT = ENDPOINT_ROOT + "scanNetworks";
export const LIST_NETWORKS_ENDPOINT = ENDPOINT_ROOT + "listNetworks";
export const WIFI_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "wifiSettings";
export const WIFI_STATUS_ENDPOINT = ENDPOINT_ROOT + "wifiStatus";
export const OTA_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "otaSettings";

const WEB_SOCKET_ROOT = process.env.REACT_APP_WEB_SOCKET_ROOT;
const WEB_SOCKET_PATH = "/ws";
const WEB_SOCKET_PREFIX = calculateWebSocketPrefix(WEB_SOCKET_PATH);

export const LIGHT_STRIP_WEBSOCKET_URI = WEB_SOCKET_PREFIX + "/lightStrip";

function calculateWebSocketPrefix(webSocketPath) {
  if (!WEB_SOCKET_ROOT || WEB_SOCKET_ROOT.length === 0){
    var loc = window.location, webSocketURI;
    if (loc.protocol === "https:") {
        webSocketURI = "wss:";
    } else {
        webSocketURI = "ws:";
    }
    return webSocketURI + "//" + loc.host + webSocketPath;
  }
  return WEB_SOCKET_ROOT + webSocketPath;
}
