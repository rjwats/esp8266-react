import { AxiosPromise } from 'axios';

import { SystemStatus } from '../types';

import { AXIOS } from './endpoints';

export function readSystemStatus(): AxiosPromise<SystemStatus> {
  return AXIOS.get('/systemStatus');
}

export function restart(): AxiosPromise<void> {
  return AXIOS.post('/restart');
}

export function factoryReset(): AxiosPromise<void> {
  return AXIOS.post('/factoryReset');
}
