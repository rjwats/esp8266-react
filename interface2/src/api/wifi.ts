import { AxiosPromise } from 'axios';

import { WiFiNetworkList, WiFiSettings, WiFiStatus } from '../types';
import { AXIOS } from './endpoints';

export function readWiFiStatus(): AxiosPromise<WiFiStatus> {
  return AXIOS.get('/wifiStatus');
}

export function scanNetworks(): AxiosPromise<void> {
  return AXIOS.get('/scanNetworks');
}

export function listNetworks(): AxiosPromise<WiFiNetworkList> {
  return AXIOS.get('/listNetworks');
}

export function readWiFiSettings(): AxiosPromise<WiFiSettings> {
  return AXIOS.get('/wifiSettings');
}

export function updateWiFiSettings(wifiSettings: WiFiSettings): AxiosPromise<WiFiSettings> {
  return AXIOS.post('/wifiSettings', wifiSettings);
}
