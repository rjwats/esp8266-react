import { Theme } from "@material-ui/core";
import { Ser2netStatus, Ser2netDisconnectReason } from "./types";

export const ser2netStatusHighlight = ({ enabled, connected }: Ser2netStatus, theme: Theme) => {
  if (!enabled) {
    return theme.palette.info.main;
  }
  if (connected) {
    return theme.palette.success.main;
  }
  return theme.palette.error.main;
}

export const ser2netStatus = ({ enabled, connected }: Ser2netStatus) => {
  if (!enabled) {
    return "Not enabled";
  }
  if (connected) {
    return "Connected";
  }
  return "Disconnected";
}

export const disconnectReason = ({ disconnect_reason }: Ser2netStatus) => {
  switch (disconnect_reason) {
    case Ser2netDisconnectReason.TCP_DISCONNECTED:
      return "TCP disconnected";
    case Ser2netDisconnectReason.SER2NET_UNACCEPTABLE_PROTOCOL_VERSION:
      return "Unacceptable protocol version";
    case Ser2netDisconnectReason.SER2NET_IDENTIFIER_REJECTED:
      return "Client ID rejected";
    case Ser2netDisconnectReason.SER2NET_SERVER_UNAVAILABLE:
      return "Server unavailable";
    case Ser2netDisconnectReason.SER2NET_MALFORMED_CREDENTIALS:
      return "Malformed credentials";
    case Ser2netDisconnectReason.SER2NET_NOT_AUTHORIZED:
      return "Not authorized";
    case Ser2netDisconnectReason.ESP8266_NOT_ENOUGH_SPACE:
      return "Device out of memory";
    case Ser2netDisconnectReason.TLS_BAD_FINGERPRINT:
      return "Server fingerprint invalid";
    default:
      return "Unknown"
  }
}
