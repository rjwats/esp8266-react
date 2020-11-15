export interface FrequencyData {
  bands: number[];
}

export enum AudioLightMode {
  OFF = "color",
  COLOR = "color",
  RAINBOW = "rainbow",
  LIGHTNING = "lightning",
  CONFETTI = "confetti",
  FIRE = "fire"
}

export interface AudioLightSettings {
  mode_id: AudioLightMode;
}
