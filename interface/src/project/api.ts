import { AxiosPromise } from "axios";

import { AXIOS } from "../api/endpoints";
import { PaletteSettings } from "./types";

export function readPaletteSettings(): AxiosPromise<PaletteSettings> {
    return AXIOS.get('/paletteSettings');
}

export function updatePaletteSettings(paletteSettings: PaletteSettings): AxiosPromise<PaletteSettings> {
    return AXIOS.post('/paletteSettings', paletteSettings);
  }
