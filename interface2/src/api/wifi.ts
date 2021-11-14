import { AxiosPromise } from 'axios';

import { WiFiStatus } from '../types';

import { AXIOS } from './endpoints';

export function readWiFiStatus(): AxiosPromise<WiFiStatus> {
  return AXIOS.get('/wifiStatus');
}
