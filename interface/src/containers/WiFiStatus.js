import React, { Component, Fragment } from 'react';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import Avatar from '@material-ui/core/Avatar';
import Divider from '@material-ui/core/Divider';
import WifiIcon from '@material-ui/icons/Wifi';
import DNSIcon from '@material-ui/icons/Dns';
import SettingsInputComponentIcon from '@material-ui/icons/SettingsInputComponent';
import SettingsInputAntennaIcon from '@material-ui/icons/SettingsInputAntenna';
import DeviceHubIcon from '@material-ui/icons/DeviceHub';
import RefreshIcon from '@material-ui/icons/Refresh';

import SectionContent from '../components/SectionContent';
import { WIFI_STATUS_ENDPOINT } from '../constants/Endpoints';
import { isConnected, connectionStatus, connectionStatusHighlight } from '../constants/WiFiConnectionStatus';
import * as Highlight from '../constants/Highlight';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';

const styles = theme => ({
  ["wifiStatus_" + Highlight.IDLE]: {
    backgroundColor: theme.palette.highlight_idle
  },
  ["wifiStatus_" + Highlight.SUCCESS]: {
    backgroundColor: theme.palette.highlight_success
  },
  ["wifiStatus_" + Highlight.ERROR]: {
    backgroundColor: theme.palette.highlight_error
  },
  ["wifiStatus_" + Highlight.WARN]: {
    backgroundColor: theme.palette.highlight_warn
  },
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

class WiFiStatus extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  dnsServers(status) {
    if (!status.dns_ip_1) {
      return "none";
    }
    return status.dns_ip_1 + (status.dns_ip_2 ? ',' + status.dns_ip_2 : '');
  }

  createListItems(data, classes) {
    return (
      <Fragment>
        <ListItem>
          <ListItemAvatar>
            <Avatar className={classes["wifiStatus_" + connectionStatusHighlight(data)]}>
              <WifiIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Connection Status" secondary={connectionStatus(data)} />
        </ListItem>
        <Divider variant="inset" component="li" />
        {
          isConnected(data) &&
          <Fragment>
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
              <ListItemText primary="Gateway IP" secondary={data.gateway_ip ? data.gateway_ip : "none"} />
            </ListItem>
            <Divider variant="inset" component="li" />
            <ListItem>
              <ListItemAvatar>
                <Avatar>
                  <DNSIcon />
                </Avatar>
              </ListItemAvatar>
              <ListItemText primary="DNS Server IP" secondary={this.dnsServers(data)} />
            </ListItem>
            <Divider variant="inset" component="li" />
          </Fragment>
        }
      </Fragment>
    );
  }

  renderWiFiStatus(data, classes) {
    return (
      <div>
        <List>
          {this.createListItems(data, classes)}
        </List>
        <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { data, fetched, errorMessage, loadData, classes } = this.props;
    return (
      <SectionContent title="WiFi Status">
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={
            () => this.renderWiFiStatus(data, classes)
          }
        />
      </SectionContent>
    );
  }

}

export default restComponent(WIFI_STATUS_ENDPOINT, withStyles(styles)(WiFiStatus));
