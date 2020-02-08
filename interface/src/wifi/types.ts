export interface WiFiStatus {
  status: number;
  local_ip: string;
  mac_address: string;
  rssi: number;
  ssid: string;
  bssid: string;
  channel: number;
  subnet_mask: string;
  gateway_ip: string;
  dns_ip_1: string;
  dns_ip_2: string;
}

export interface WiFiSettings {
  ssid: string;
  password: string;
  hostname: string;
  static_ip_config: boolean;
  local_ip?: string;
  gateway_ip?: string;
  subnet_mask?: string;
  dns_ip_1?: string;
  dns_ip_2?: string;
}

export interface WiFiNetworkList {
  networks: WiFiNetwork[];
}

export interface WiFiNetwork {
  rssi: number;
  ssid: string;
  bssid: string;
  channel: number;
  encryption_type: number;
}
