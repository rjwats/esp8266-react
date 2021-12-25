import { AxiosPromise } from 'axios';

import { OTASettings, SystemStatus } from '../types';
import { AXIOS, FileUploadConfig, uploadFile } from './endpoints';

export function readSystemStatus(timeout?: number): AxiosPromise<SystemStatus> {
  return AXIOS.get('/systemStatus', { timeout });
}

export function restart(): AxiosPromise<void> {
  return AXIOS.post('/restart');
}

export function factoryReset(): AxiosPromise<void> {
  return AXIOS.post('/factoryReset');
}

export function readOTASettings(): AxiosPromise<OTASettings> {
  return AXIOS.get('/otaSettings');
}

export function updateOTASettings(otaSettings: OTASettings): AxiosPromise<OTASettings> {
  return AXIOS.post('/otaSettings', otaSettings);
}

export const uploadFirmware = (file: File, config?: FileUploadConfig): AxiosPromise<void> => (
  uploadFile('/uploadFirmware', file, config)
);
