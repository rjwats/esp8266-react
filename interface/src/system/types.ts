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
}

export interface OTASettings {
  enabled: boolean;
  port: number;
  password: string;
}

export enum LogLevel {
  DEBUG = 0,
  INFO = 1,
  WARNING = 2,
  ERROR = 3
}


export interface LogEvent {
  time: string;
  level: LogLevel;
  file: string;
  line: number;
  message: string;
}
