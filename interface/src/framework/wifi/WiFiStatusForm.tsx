import { FC } from "react";

import { Avatar, Button, Divider, List, ListItem, ListItemAvatar, ListItemText, Theme, useTheme } from "@mui/material";
import SettingsInputComponentIcon from '@mui/icons-material/SettingsInputComponent';
import SettingsInputAntennaIcon from '@mui/icons-material/SettingsInputAntenna';
import DeviceHubIcon from '@mui/icons-material/DeviceHub';
import WifiIcon from '@mui/icons-material/Wifi';
import DnsIcon from '@mui/icons-material/Dns';
import RefreshIcon from '@mui/icons-material/Refresh';

import * as WiFiApi from "../../api/wifi";
import { WiFiConnectionStatus, WiFiStatus } from "../../types";
import { ButtonRow, FormLoader, SectionContent } from "../../components";
import { useRest } from "../../utils";

const isConnected = ({ status }: WiFiStatus) => status === WiFiConnectionStatus.WIFI_STATUS_CONNECTED;

const wifiStatusHighlight = ({ status }: WiFiStatus, theme: Theme) => {
  switch (status) {
    case WiFiConnectionStatus.WIFI_STATUS_IDLE:
    case WiFiConnectionStatus.WIFI_STATUS_DISCONNECTED:
    case WiFiConnectionStatus.WIFI_STATUS_NO_SHIELD:
      return theme.palette.info.main;
    case WiFiConnectionStatus.WIFI_STATUS_CONNECTED:
      return theme.palette.success.main;
    case WiFiConnectionStatus.WIFI_STATUS_CONNECT_FAILED:
    case WiFiConnectionStatus.WIFI_STATUS_CONNECTION_LOST:
      return theme.palette.error.main;
    default:
      return theme.palette.warning.main;
  }
};

const wifiStatus = ({ status }: WiFiStatus) => {
  switch (status) {
    case WiFiConnectionStatus.WIFI_STATUS_NO_SHIELD:
      return "Inactive";
    case WiFiConnectionStatus.WIFI_STATUS_IDLE:
      return "Idle";
    case WiFiConnectionStatus.WIFI_STATUS_NO_SSID_AVAIL:
      return "No SSID Available";
    case WiFiConnectionStatus.WIFI_STATUS_CONNECTED:
      return "Connected";
    case WiFiConnectionStatus.WIFI_STATUS_CONNECT_FAILED:
      return "Connection Failed";
    case WiFiConnectionStatus.WIFI_STATUS_CONNECTION_LOST:
      return "Connection Lost";
    case WiFiConnectionStatus.WIFI_STATUS_DISCONNECTED:
      return "Disconnected";
    default:
      return "Unknown";
  }
};

const dnsServers = ({ dns_ip_1, dns_ip_2 }: WiFiStatus) => {
  if (!dns_ip_1) {
    return "none";
  }
  return dns_ip_1 + (dns_ip_2 ? ',' + dns_ip_2 : '');
};

const WiFiStatusForm: FC = () => {
  const {
    loadData, data, errorMessage
  } = useRest<WiFiStatus>({ read: WiFiApi.readWiFiStatus });

  const theme = useTheme();

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <List>
          <ListItem>
            <ListItemAvatar>
              <Avatar sx={{ bgcolor: wifiStatusHighlight(data, theme) }}>
                <WifiIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Status" secondary={wifiStatus(data)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          {
            isConnected(data) &&
            <>
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <SettingsInputAntennaIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="SSID" secondary={data.ssid} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>IP</Avatar>
                </ListItemAvatar>
                <ListItemText primary="IP Address" secondary={data.local_ip} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <DeviceHubIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="MAC Address" secondary={data.mac_address} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>#</Avatar>
                </ListItemAvatar>
                <ListItemText primary="Subnet Mask" secondary={data.subnet_mask} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <SettingsInputComponentIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="Gateway IP" secondary={data.gateway_ip || "none"} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <DnsIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="DNS Server IP" secondary={dnsServers(data)} />
              </ListItem>
              <Divider variant="inset" component="li" />
            </>
          }
        </List>
        <ButtonRow pt={1}>
          <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" onClick={loadData}>
            Refresh
          </Button>
        </ButtonRow>
      </>
    );
  };

  return (
    <SectionContent title='WiFi Status' titleGutter>
      {content()}
    </SectionContent>
  );

};

export default WiFiStatusForm;
