export enum ApProvisionMode {
  AP_MODE_ALWAYS = 0,
  AP_MODE_DISCONNECTED = 1,
  AP_NEVER = 2
}

export interface APStatus {
  active: boolean;
  ip_address: string;
  mac_address: string;
  station_num: number;
}

export interface APSettings {
  provision_mode: ApProvisionMode;
  ssid: string;
  password: string;
}
