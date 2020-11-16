export interface FrequencyData {
  bands: number[];
}

export enum AudioLightModeType {
  OFF = "off",
  COLOR = "color",
  RAINBOW = "rainbow",
  LIGHTNING = "lightning",
  CONFETTI = "confetti",
  FIRE = "fire"
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
  max_changes: number;
  brightness: number;
  delay: number;
}

export interface FireMode {
  mode_id: AudioLightModeType.FIRE;
  cooling: number;
  spaking: number;
  reverse: boolean;
}

export type AudioLightMode = OffMode | ColorMode | RainbowMode | LightningMode | ConfettiMode | FireMode;
