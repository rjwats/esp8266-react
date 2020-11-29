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

export interface Palette {
  name: string;
  colors: string[];
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

export interface OffModeSettings {
}

export interface ColorModeSettings {
  color: string;
  brightness: number;
  audio_enabled: boolean;
  included_bands: boolean[]
}

export interface RainbowModeSettings {
  brightness: number;
  rotate_speed: 32;
  audio_enabled: boolean;
  hue_delta: number;
}

export interface LightningModeSettings {
  color: string;
  brightness: number;
  threshold: number;
  flashes: number;
  audio_enabled: boolean;
  included_bands: boolean[]
}

export interface ConfettiModeSettings {
  palette1: string;
  palette2: string;
  palette3: string;
  max_changes: number;
  brightness: number;
  delay: number;
}

export interface FireModeSettings {
  palette: string;
  cooling: number;
  sparking: number;
  reverse: boolean;
}

export interface PacificaModeSettings {
  palette1: string;
  palette2: string;
  palette3: string;
}

export interface PrideModeSettings {
  brightness_bpm: number;
  brightness_freq_min: number;
  brightness_freq_max: number;
  hue_bpm: number;
  hue_delta_min: number;
  hue_delta_max: number;
}

export interface RotateModeSettings {
  modes: AudioLightModeType[];
  delay: number;
}

export interface PaletteSettings {
  palettes: Palette[];
}

export interface LedSettings {
  max_power_milliwatts: number;
  dead_zone: number;
  smoothing_factor: number;
}

export type AudioLightModeSettings = (
  OffModeSettings |
  ColorModeSettings |
  RainbowModeSettings |
  LightningModeSettings |
  ConfettiModeSettings |
  FireModeSettings |
  PacificaModeSettings |
  PrideModeSettings |
  RotateModeSettings
)

export interface AudioLightMode {
  mode_id: AudioLightModeType;
  settings: AudioLightModeSettings;
}

export const LED_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "ledSettings";
export const PALETTE_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "paletteSettings";
export const AUDIO_LIGHT_SAVE_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "saveModeSettings";
export const AUDIO_LIGHT_LOAD_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "loadModeSettings";
export const AUDIO_LIGHT_SETTINGS_ENDPOINT = WEB_SOCKET_ROOT + "audioLightSettings";

export const generateGradient = (palette: Palette) => {
  return `linear-gradient(0.25turn, ${palette.colors.join(', ')})`;
}

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

export interface AudioLightModeMetadata {
  label: string;
  renderer?: React.ComponentType<WebSocketFormProps<AudioLightMode>>;
  rotate: boolean;
}

export const AUDIO_LIGHT_MODE_METADATA: { [type in AudioLightModeType]: AudioLightModeMetadata } = {
  off: {
    label: "Off",
    rotate: false
  },
  color: {
    label: "Color",
    renderer: AudioLightColorMode,
    rotate: true
  },
  rainbow: {
    label: "Rainbow",
    renderer: AudioLightRainbowMode,
    rotate: true
  },
  lightning: {
    label: "Lightning",
    renderer: AudioLightLightningMode,
    rotate: true
  },
  confetti: {
    label: "Confetti",
    renderer: AudioLightConfettiMode,
    rotate: true
  },
  fire: {
    label: "Fire",
    renderer: AudioLightFireMode,
    rotate: true
  },
  pacifica: {
    label: "Pacifica",
    renderer: AudioLightPacificaMode,
    rotate: true
  },
  pride: {
    label: "Pride",
    renderer: AudioLightPrideMode,
    rotate: true
  },
  rotate: {
    label: "Rotate",
    renderer: AudioLightRotateMode,
    rotate: false
  }
}

export const AUDIO_LIGHT_MODES = Object.entries(AudioLightModeType).map(value => value[1]);
export const ROTATE_AUDIO_LIGHT_MODES = Object.entries(AudioLightModeType).map(value => value[1]).filter(value => AUDIO_LIGHT_MODE_METADATA[value].rotate);
