import React, { Component, Fragment } from 'react';

import { withStyles, createStyles, Theme, WithStyles } from '@material-ui/core/styles';
import { Avatar, Divider, List, ListItem, ListItemAvatar, ListItemText } from '@material-ui/core';

import DNSIcon from '@material-ui/icons/Dns';
import WifiIcon from '@material-ui/icons/Wifi';
import SettingsInputComponentIcon from '@material-ui/icons/SettingsInputComponent';
import SettingsInputAntennaIcon from '@material-ui/icons/SettingsInputAntenna';
import DeviceHubIcon from '@material-ui/icons/DeviceHub';
import RefreshIcon from '@material-ui/icons/Refresh';

import { RestFormProps } from '../components/RestFormLoader';
import { wifiStatus, wifiStatusHighlight, isConnected } from '../constants/WiFiStatus';
import { WiFiStatusData } from '../containers/WiFiStatus';
import FormActions from '../components/FormActions';
import FormButton from '../components/FormButton';

const styles = (theme: Theme) => createStyles({
  wifiStatusHighlight: ({ data }: RestFormProps<WiFiStatusData>) => ({
    backgroundColor: wifiStatusHighlight(data, theme)
  })
});

type WiFiStatusFormProps = RestFormProps<WiFiStatusData> & WithStyles<typeof styles>;

class WiFiStatusForm extends Component<WiFiStatusFormProps> {

  dnsServers(status: WiFiStatusData) {
    if (!status.dns_ip_1) {
      return "none";
    }
    return status.dns_ip_1 + (status.dns_ip_2 ? ',' + status.dns_ip_2 : '');
  }

  createListItems() {
    const { data, classes } = this.props
    return (
      <Fragment>
        <ListItem>
          <ListItemAvatar>
            <Avatar className={classes.wifiStatusHighlight}>
              <WifiIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Status" secondary={wifiStatus(data)} />
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

  render() {
    return (
      <div>
        <List>
          {this.createListItems()}
        </List>
        <FormActions>
          <FormButton startIcon={<RefreshIcon />} variant="contained" color="secondary" onClick={this.props.loadData}>
            Refresh
          </FormButton>
        </FormActions>
      </div>
    );
  }

}

export default withStyles(styles)(WiFiStatusForm);
