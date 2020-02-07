export interface APStatus {
  active: boolean;
  ip_address: string;
  mac_address: string;
  station_num: number;
}

export interface APSettings {
  provision_mode: number;
  ssid: string;
  password: string;
}
