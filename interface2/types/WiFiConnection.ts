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

