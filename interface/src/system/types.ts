export interface SystemStatus {
  esp_platform: string;
  max_alloc_heap: number;
  cpu_freq_mhz: number;
  free_heap: number;
  sketch_size: number;
  free_sketch_space: number;
  sdk_version: string;
  flash_chip_size: number;
  flash_chip_speed: number;
  spiffs_used: number;
  spiffs_total: number;
}

export interface OTASettings {
  enabled: boolean;
  port: number;
  password: string;
}
