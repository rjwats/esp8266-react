import { Theme } from "@material-ui/core";
import { MQTTStatus, MQTTDisconnectReason } from "./types";

export const mqttStatusHighlight = ({ connected }: MQTTStatus, theme: Theme) => {
  if (connected) {
    return theme.palette.success.main;
  }
  return theme.palette.info.main;
}

export const mqttStatus = ({ connected }: MQTTStatus) => {
  if (connected) {
    return "Connected";
  }
  return "Disconnected";
}

export const disconnectReason = ({ disconnect_reason }: MQTTStatus) => {
  switch (disconnect_reason) {
    case MQTTDisconnectReason.TCP_DISCONNECTED:
      return "TCP disconnected";
    case MQTTDisconnectReason.MQTT_UNACCEPTABLE_PROTOCOL_VERSION:
      return "Unacceptable protocol version";
    case MQTTDisconnectReason.MQTT_IDENTIFIER_REJECTED:
      return "Client ID rejected";
    case MQTTDisconnectReason.MQTT_SERVER_UNAVAILABLE:
      return "Server unavailable";
    case MQTTDisconnectReason.MQTT_MALFORMED_CREDENTIALS:
      return "Malformed credentials";
    case MQTTDisconnectReason.MQTT_NOT_AUTHORIZED:
      return "Not authorized";
    case MQTTDisconnectReason.ESP8266_NOT_ENOUGH_SPACE:
      return "Device out of memory";
    case MQTTDisconnectReason.TLS_BAD_FINGERPRINT:
      return "Server fingerprint invalid";
    default:
      return "Unknown"
  }
}
