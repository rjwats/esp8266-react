import { AxiosPromise } from "axios";

import { AXIOS } from "../api/endpoints";
import { LedSettings, PaletteSettings } from "./types";

export function readPaletteSettings(): AxiosPromise<PaletteSettings> {
  return AXIOS.get('/paletteSettings');
}

export function updatePaletteSettings(paletteSettings: PaletteSettings): AxiosPromise<PaletteSettings> {
  return AXIOS.post('/paletteSettings', paletteSettings);
}

export function readLedSettings(): AxiosPromise<LedSettings> {
  return AXIOS.get('/ledSettings');
}

export function updateLedSettings(ledSettings: LedSettings): AxiosPromise<LedSettings> {
  return AXIOS.post('/ledSettings', ledSettings);
}

export function loadModeSettings(): AxiosPromise<void> {
  return AXIOS.post('/loadModeSettings');
}

export function saveModeSettings(): AxiosPromise<void> {
  return AXIOS.post('/saveModeSettings');
}
