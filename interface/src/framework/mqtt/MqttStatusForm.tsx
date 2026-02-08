import { FC } from "react";

import { Avatar, Button, Divider, List, ListItem, ListItemAvatar, ListItemText, Theme, useTheme } from "@mui/material";
import DeviceHubIcon from '@mui/icons-material/DeviceHub';
import RefreshIcon from '@mui/icons-material/Refresh';
import ReportIcon from '@mui/icons-material/Report';

import * as MqttApi from "../../api/mqtt";
import { MqttStatus, MqttDisconnectReason } from "../../types";
import { ButtonRow, FormLoader, SectionContent } from "../../components";
import { useRest } from "../../utils";

const mqttStatusHighlight = ({ enabled, connected }: MqttStatus, theme: Theme) => {
  if (!enabled) {
    return theme.palette.info.main;
  }
  if (connected) {
    return theme.palette.success.main;
  }
  return theme.palette.error.main;
};

const mqttStatus = ({ enabled, connected }: MqttStatus) => {
  if (!enabled) {
    return "Not enabled";
  }
  if (connected) {
    return "Connected";
  }
  return "Disconnected";
};

const disconnectReason = ({ disconnect_reason }: MqttStatus) => {
  switch (disconnect_reason) {
    case MqttDisconnectReason.USER_OK:
      return "User disconnected";

    case MqttDisconnectReason.MQTT_UNACCEPTABLE_PROTOCOL_VERSION:
      return "Unacceptable protocol version";
    case MqttDisconnectReason.MQTT_IDENTIFIER_REJECTED:
      return "Client ID rejected";
    case MqttDisconnectReason.MQTT_SERVER_UNAVAILABLE:
      return "Server unavailable";
    case MqttDisconnectReason.MQTT_MALFORMED_CREDENTIALS:
      return "Malformed credentials";
    case MqttDisconnectReason.MQTT_NOT_AUTHORIZED:
      return "Not authorized";
    case MqttDisconnectReason.TLS_BAD_FINGERPRINT:
      return "Server fingerprint invalid";
    case MqttDisconnectReason.TCP_DISCONNECTED:
      return "TCP disconnected";
    default:
      return `Unknown error (${disconnect_reason})`;
  }
};

const MqttStatusForm: FC = () => {
  const { loadData, data, errorMessage } = useRest<MqttStatus>({ read: MqttApi.readMqttStatus });

  const theme = useTheme();

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    const renderConnectionStatus = () => {
      if (data.connected) {
        return (
          <>
            <ListItem>
              <ListItemAvatar>
                <Avatar>#</Avatar>
              </ListItemAvatar>
              <ListItemText primary="Client ID" secondary={data.client_id} />
            </ListItem>
            <Divider variant="inset" component="li" />
          </>
        );
      }
      return (
        <>
          <ListItem>
            <ListItemAvatar>
              <Avatar>
                <ReportIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Disconnect Reason" secondary={disconnectReason(data)} />
          </ListItem>
          <Divider variant="inset" component="li" />
        </>
      );
    };

    return (
      <>
        <List>
          <ListItem>
            <ListItemAvatar>
              <Avatar sx={{ bgcolor: mqttStatusHighlight(data, theme) }}>
                <DeviceHubIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Status" secondary={mqttStatus(data)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          {data.enabled && renderConnectionStatus()}
        </List >
        <ButtonRow pt={1}>
          <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" onClick={loadData}>
            Refresh
          </Button>
        </ButtonRow>
      </>
    );
  };

  return (
    <SectionContent title='MQTT Status' titleGutter>
      {content()}
    </SectionContent>
  );

};

export default MqttStatusForm;
