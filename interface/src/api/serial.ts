import { AxiosPromise } from "axios";

import { SerialSettings, SerialStatus } from "../types";
import { AXIOS } from "./endpoints";

export function readSerialStatus(): AxiosPromise<SerialStatus> {
  return AXIOS.get('/serialStatus');
}

export function readSerialSettings(): AxiosPromise<SerialSettings> {
  return AXIOS.get('/serialSettings');
}

export function updateSerialSettings(serialSettings: SerialSettings): AxiosPromise<SerialSettings> {
  return AXIOS.post('/serialSettings', serialSettings);
}
