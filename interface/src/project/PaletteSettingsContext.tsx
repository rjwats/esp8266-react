import React from "react";
import { PaletteSettings } from "./types";

export type PaletteSettingsContextType = {
  paletteSettingsUpdated: (paletteSettings: PaletteSettings) => void;
  paletteSettings: PaletteSettings;
}

const PaletteSettingsContextDefaultValue = {} as PaletteSettingsContextType;
export const PaletteSettingsContext = React.createContext(
  PaletteSettingsContextDefaultValue
);
