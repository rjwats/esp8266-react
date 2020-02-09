export interface SystemStatus {
  esp_platform: string;
  cpu_freq_mhz: number;
  free_heap: number;
  sketch_size: number;
  free_sketch_space: number;
  flash_chip_size: number;
}

export interface OTASettings {
  enabled: boolean;
  port: number;
  password: string;
}
