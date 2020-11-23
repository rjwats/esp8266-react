import { ENDPOINT_ROOT, WEB_SOCKET_ROOT } from "../api";

export interface FrequencyData {
  bands: number[];
}

export enum AudioLightModeType {
  OFF = "off",
  COLOR = "color",
  RAINBOW = "rainbow",
  LIGHTNING = "lightning",
  CONFETTI = "confetti",
  FIRE = "fire",
  PACIFICA = "pacifica",
  PRIDE = "pride"
}

export interface OffMode {
  mode_id: AudioLightModeType.OFF;
}

export interface ColorMode {
  mode_id: AudioLightModeType.COLOR;
  color: string;
  brightness: number;
  audio_enabled: boolean;
  included_bands: boolean[]
}

export interface RainbowMode {
  mode_id: AudioLightModeType.RAINBOW;
  brightness: number;
  rotate_speed: 32;
  audio_enabled: boolean;
  hue_delta: number;
}

export interface LightningMode {
  mode_id: AudioLightModeType.LIGHTNING;
  color: string;
  brightness: number;
  threshold: number;
  flashes: number;
  audio_enabled: boolean;
  included_bands: boolean[]
}

export interface ConfettiMode {
  mode_id: AudioLightModeType.CONFETTI;
  palette1: string;
  palette2: string;
  palette3: string;
  max_changes: number;
  brightness: number;
  delay: number;
}

export interface FireMode {
  mode_id: AudioLightModeType.FIRE;
  palette: string;
  cooling: number;
  sparking: number;
  reverse: boolean;
}

export interface PacificaMode {
  mode_id: AudioLightModeType.PACIFICA;
  palette1: string;
  palette2: string;
  palette3: string;
}

export interface PrideMode {
  mode_id: AudioLightModeType.PRIDE;
}

export interface PaletteSettings {
  palettes: Palette[];
}

export interface Palette {
  id: string;
  colors: string[];
};

export type AudioLightMode = OffMode | ColorMode | RainbowMode | LightningMode | ConfettiMode | FireMode | PacificaMode | PrideMode;

export const PALETTE_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "paletteSettings";
export const AUDIO_LIGHT_SAVE_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "saveModeSettings";
export const AUDIO_LIGHT_LOAD_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "loadModeSettings";
export const AUDIO_LIGHT_SETTINGS_ENDPOINT = WEB_SOCKET_ROOT + "audioLightSettings";

export const generateGradient = (palette: Palette) => {
  return `linear-gradient(0.25turn, ${palette.colors.join(', ')})`;
};

export const DEFAULT_PALETTE = [
  "#ff0000",
  "#d52a00",
  "#ab5500",
  "#ab7f00",
  "#abab00",
  "#56d500",
  "#00ff00",
  "#00d52a",
  "#00ab55",
  "#0056aa",
  "#0000ff",
  "#2a00d5",
  "#5500ab",
  "#7f0081",
  "#ab0055",
  "#d5002b"
];
