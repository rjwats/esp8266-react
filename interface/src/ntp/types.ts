export interface NTPStatus {
  status: number;
  time_utc: string;
  time_local: string;
  server: string;
  uptime: number;
}

export interface NTPSettings {
  enabled: boolean;
  server: string;
  tz_label: string;
  tz_format: string;
}
