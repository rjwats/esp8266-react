import { Theme } from "@material-ui/core";
import { SerialStatus  } from "./types";

export const serialStatusHighlight = ({ enabled }: SerialStatus, theme: Theme) => {
  if (!enabled) {
    return theme.palette.info.main;
  }
  return theme.palette.success.main;
}

export const serialStatus = ({ enabled  }: SerialStatus) => {
  if (!enabled) {
    return "Not enabled";
  }
  return "Enabled";
}

// export const disconnectReason = ({ disconnect_reason }: SerialStatus) => {
//   switch (disconnect_reason) {
//     case SerialDisconnectReason.TCP_DISCONNECTED:
//       return "TCP disconnected";
//     case SerialDisconnectReason.SERIAL_UNACCEPTABLE_PROTOCOL_VERSION:
//       return "Unacceptable protocol version";
//     case SerialDisconnectReason.SERIAL_IDENTIFIER_REJECTED:
//       return "Client ID rejected";
//     case SerialDisconnectReason.SERIAL_SERVER_UNAVAILABLE:
//       return "Server unavailable";
//     case SerialDisconnectReason.SERIAL_MALFORMED_CREDENTIALS:
//       return "Malformed credentials";
//     case SerialDisconnectReason.SERIAL_NOT_AUTHORIZED:
//       return "Not authorized";
//     case SerialDisconnectReason.ESP8266_NOT_ENOUGH_SPACE:
//       return "Device out of memory";
//     case SerialDisconnectReason.TLS_BAD_FINGERPRINT:
//       return "Server fingerprint invalid";
//     default:
//       return "Unknown"
//   }
// }
