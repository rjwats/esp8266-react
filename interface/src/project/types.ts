import { ENDPOINT_ROOT, WEB_SOCKET_ROOT } from "../api";
import { WebSocketFormProps } from "../components";

import AudioLightColorMode from "./modes/AudioLightColorMode";
import AudioLightConfettiMode from "./modes/AudioLightConfettiMode";
import AudioLightFireMode from "./modes/AudioLightFireMode";
import AudioLightLightningMode from "./modes/AudioLightLightningMode";
import AudioLightPacificaMode from "./modes/AudioLightPacificaMode";
import AudioLightPrideMode from "./modes/AudioLightPrideMode";
import AudioLightRainbowMode from "./modes/AudioLightRainbowMode";
import AudioLightRotateMode from "./modes/AudioLightRotateMode";

export interface FrequencyData {
  bands: number[];
}

export interface AudioLightModeMetadata<M extends AudioLightMode> {
  label: string;
  renderer?: React.ComponentType<WebSocketFormProps<M>>;
  rotate: boolean;
}

export enum AudioLightModeType {
  OFF = "off",
  COLOR = "color",
  RAINBOW = "rainbow",
  LIGHTNING = "lightning",
  CONFETTI = "confetti",
  FIRE = "fire",
  PACIFICA = "pacifica",
  PRIDE = "pride",
  ROTATE = "rotate"
}

export interface OffMode {
  mode_id: AudioLightModeType.OFF;
}

export const OFF_MODE_METADATA: AudioLightModeMetadata<OffMode> = {
  label: "Off",
  rotate: false
};

export interface ColorMode {
  mode_id: AudioLightModeType.COLOR;
  color: string;
  brightness: number;
  audio_enabled: boolean;
  included_bands: boolean[]
}

export const COLOR_MODE_METADATA: AudioLightModeMetadata<ColorMode> = {
  label: "Color",
  renderer: AudioLightColorMode,
  rotate: true
};

export interface RainbowMode {
  mode_id: AudioLightModeType.RAINBOW;
  brightness: number;
  rotate_speed: 32;
  audio_enabled: boolean;
  hue_delta: number;
}

export const RAINBOW_MODE_METADATA: AudioLightModeMetadata<RainbowMode> = {
  label: "Rainbow",
  renderer: AudioLightRainbowMode,
  rotate: true
};

export interface LightningMode {
  mode_id: AudioLightModeType.LIGHTNING;
  color: string;
  brightness: number;
  threshold: number;
  flashes: number;
  audio_enabled: boolean;
  included_bands: boolean[]
}

export const LIGHTNING_MODE_METADATA: AudioLightModeMetadata<LightningMode> = {
  label: "Lightning",
  renderer: AudioLightLightningMode,
  rotate: true
};

export interface ConfettiMode {
  mode_id: AudioLightModeType.CONFETTI;
  palette1: string;
  palette2: string;
  palette3: string;
  max_changes: number;
  brightness: number;
  delay: number;
}

export const CONFETTI_MODE_METADATA: AudioLightModeMetadata<ConfettiMode> = {
  label: "Confetti",
  renderer: AudioLightConfettiMode,
  rotate: true
};

export interface FireMode {
  mode_id: AudioLightModeType.FIRE;
  palette: string;
  cooling: number;
  sparking: number;
  reverse: boolean;
}

export const FIRE_MODE_METADATA: AudioLightModeMetadata<FireMode> = {
  label: "Fire",
  renderer: AudioLightFireMode,
  rotate: true
};

export interface PacificaMode {
  mode_id: AudioLightModeType.PACIFICA;
  palette1: string;
  palette2: string;
  palette3: string;
}

export const PACIFICA_MODE_METADATA: AudioLightModeMetadata<PacificaMode> = {
  label: "Pacifica",
  renderer: AudioLightPacificaMode,
  rotate: true
};

export interface PrideMode {
  mode_id: AudioLightModeType.PRIDE;
  brightness_bpm: number;
  brightness_freq_min: number;
  brightness_freq_max: number;
  hue_bpm: number;
  hue_delta_min: number;
  hue_delta_max: number;
}

export const PRIDE_MODE_METADATA: AudioLightModeMetadata<PrideMode> = {
  label: "Pride",
  renderer: AudioLightPrideMode,
  rotate: true
};

export interface RotateMode {
  mode_id: AudioLightModeType.ROTATE;
  modes: AudioLightModeType[];
}

export const ROTATE_MODE_METADATA: AudioLightModeMetadata<RotateMode> = {
  label: "Rotate",
  renderer: AudioLightRotateMode,
  rotate: false
};

export interface PaletteSettings {
  palettes: Palette[];
}

export interface Palette {
  id: string;
  colors: string[];
};

export type AudioLightMode = (
  OffMode |
  ColorMode |
  RainbowMode |
  LightningMode |
  ConfettiMode |
  FireMode |
  PacificaMode |
  PrideMode |
  RotateMode
);

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

export const AUDIO_LIGHT_MODE_METADATA: { [type in AudioLightModeType]: AudioLightModeMetadata<any> } = {
  off: OFF_MODE_METADATA,
  color: COLOR_MODE_METADATA,
  rainbow: RAINBOW_MODE_METADATA,
  lightning: LIGHTNING_MODE_METADATA,
  confetti: CONFETTI_MODE_METADATA,
  fire: FIRE_MODE_METADATA,
  pacifica: PACIFICA_MODE_METADATA,
  pride: PRIDE_MODE_METADATA,
  rotate: ROTATE_MODE_METADATA
}
