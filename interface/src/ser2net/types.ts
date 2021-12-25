export enum Ser2netDisconnectReason {
  TCP_DISCONNECTED = 0,
  SER2NET_UNACCEPTABLE_PROTOCOL_VERSION = 1,
  SER2NET_IDENTIFIER_REJECTED = 2,
  SER2NET_SERVER_UNAVAILABLE = 3,
  SER2NET_MALFORMED_CREDENTIALS = 4,
  SER2NET_NOT_AUTHORIZED = 5,
  ESP8266_NOT_ENOUGH_SPACE = 6,
  TLS_BAD_FINGERPRINT = 7
}

export interface Ser2netStatus {
  enabled: boolean;
  connected: boolean;
  client_id: string;
  disconnect_reason: Ser2netDisconnectReason;
}

export interface Ser2netSettings {
  enabled: boolean;
  host: string;
  port: number;
  username: string;
  password: string;
  client_id: string;
  keep_alive: number;
  clean_session: boolean;
  max_topic_length: number;
}
