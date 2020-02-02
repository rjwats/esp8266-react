import { Theme } from '@material-ui/core';
import { WiFiStatus } from '../containers/WiFiStatusController';

export const WIFI_STATUS_IDLE = 0;
export const WIFI_STATUS_NO_SSID_AVAIL = 1;
export const WIFI_STATUS_CONNECTED = 3;
export const WIFI_STATUS_CONNECT_FAILED = 4;
export const WIFI_STATUS_CONNECTION_LOST = 5;
export const WIFI_STATUS_DISCONNECTED = 6;

export const isConnected = ({ status }: WiFiStatus) => status === WIFI_STATUS_CONNECTED;

export const wifiStatusHighlight = ({ status }: WiFiStatus, theme: Theme) => {
  switch (status) {
    case WIFI_STATUS_IDLE:
    case WIFI_STATUS_DISCONNECTED:
      return theme.palette.info.main;
    case WIFI_STATUS_CONNECTED:
      return theme.palette.success.main;
    case WIFI_STATUS_CONNECT_FAILED:
    case WIFI_STATUS_CONNECTION_LOST:
      return theme.palette.error.main;
    default:
      return theme.palette.warning.main;
  }
}

export const wifiStatus = ({ status }: WiFiStatus) => {
  switch (status) {
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
