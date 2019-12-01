import { ENDPOINT_ROOT, WEB_SOCKET_ROOT } from '../constants/Env';

export const NTP_STATUS_ENDPOINT = ENDPOINT_ROOT + "ntpStatus";
export const NTP_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "ntpSettings";
export const AP_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "apSettings";
export const AP_STATUS_ENDPOINT = ENDPOINT_ROOT + "apStatus";
export const SCAN_NETWORKS_ENDPOINT = ENDPOINT_ROOT + "scanNetworks";
export const LIST_NETWORKS_ENDPOINT = ENDPOINT_ROOT + "listNetworks";
export const WIFI_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "wifiSettings";
export const WIFI_STATUS_ENDPOINT = ENDPOINT_ROOT + "wifiStatus";
export const OTA_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "otaSettings";
export const SYSTEM_STATUS_ENDPOINT = ENDPOINT_ROOT + "systemStatus";
export const SIGN_IN_ENDPOINT = ENDPOINT_ROOT + "signIn";
export const VERIFY_AUTHORIZATION_ENDPOINT = ENDPOINT_ROOT + "verifyAuthorization";
export const SECURITY_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "securitySettings";
export const RESTART_ENDPOINT = ENDPOINT_ROOT + "restart";

const WEB_SOCKET_PATH = "/ws";
const WEB_SOCKET_PREFIX = calculateWebSocketPrefix(WEB_SOCKET_PATH);

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

export const FREQUENCY_STREAM_WEBSOCKET_URI = WEB_SOCKET_PREFIX + "/frequencyStream";
export const AUDIO_LIGHT_WEBSOCKET_URI = WEB_SOCKET_PREFIX + "/audioLight";
