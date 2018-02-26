import * as Highlight from '../constants/Highlight';

export const WIFI_STATUS_IDLE = 0;
export const WIFI_STATUS_NO_SSID_AVAIL = 1;
export const WIFI_STATUS_CONNECTED = 3;
export const WIFI_STATUS_CONNECT_FAILED = 4;
export const WIFI_STATUS_CONNECTION_LOST = 5;
export const WIFI_STATUS_DISCONNECTED = 6;

export const isConnected = wifiStatus => wifiStatus && wifiStatus.status === WIFI_STATUS_CONNECTED;

export const connectionStatusHighlight = wifiStatus => {
  switch (wifiStatus.status){
    case WIFI_STATUS_IDLE:
    case WIFI_STATUS_DISCONNECTED:
      return Highlight.IDLE;
    case WIFI_STATUS_CONNECTED:
      return Highlight.SUCCESS;
    case WIFI_STATUS_CONNECT_FAILED:
    case WIFI_STATUS_CONNECTION_LOST:
      return Highlight.ERROR;
    default:
      return Highlight.WARN;
  }
}

export const connectionStatus = wifiStatus => {
  switch (wifiStatus.status){
    case WIFI_STATUS_IDLE:
      return "Idle";
    case WIFI_STATUS_NO_SSID_AVAIL:
      return "No SSID Available";
    case WIFI_STATUS_CONNECTED:
      return "Connected";
    case WIFI_STATUS_CONNECT_FAILED:
      return "Connection Failed";
    case WIFI_STATUS_CONNECTION_LOST:
      return "Connection Lost";
    case WIFI_STATUS_DISCONNECTED:
      return "Disconnected";
    default:
      return "Unknown";
  }
}
