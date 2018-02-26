import React, { Component } from 'react';

import { withStyles } from 'material-ui/styles';
import Button from 'material-ui/Button';
import { LinearProgress } from 'material-ui/Progress';
import Typography from 'material-ui/Typography';

import SnackbarNotification from '../components/SnackbarNotification';
import SectionContent from '../components/SectionContent';

import { WIFI_STATUS_ENDPOINT }  from  '../constants/Endpoints';
import { isConnected, connectionStatus, connectionStatusHighlight }  from  '../constants/WiFiConnectionStatus';
import * as Highlight from '../constants/Highlight';

import { simpleGet }  from  '../helpers/SimpleGet';

import List, { ListItem, ListItemText } from 'material-ui/List';
import Avatar from 'material-ui/Avatar';
import Divider from 'material-ui/Divider';
import WifiIcon from 'material-ui-icons/Wifi';
import DNSIcon from 'material-ui-icons/Dns';
import SettingsInputComponentIcon from 'material-ui-icons/SettingsInputComponent';
import SettingsInputAntennaIcon from 'material-ui-icons/SettingsInputAntenna';

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
  fetching: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class WiFiStatus extends Component {

  constructor(props) {
    super(props);

    this.state = {
             status:null,
             fetched: false,
             errorMessage:null
           };

    this.setState = this.setState.bind(this);
    this.loadWiFiStatus = this.loadWiFiStatus.bind(this);
  }

  componentDidMount() {
    this.loadWiFiStatus();
  }

  dnsServers(status) {
    if (!status.dns_ip_1){
      return "none";
    }
    return status.dns_ip_1 + (status.dns_ip_2 ? ','+status.dns_ip_2 : '');
  }

  loadWiFiStatus() {
    simpleGet(
      WIFI_STATUS_ENDPOINT,
      this.setState,
      this.raiseNotification
    );
  }

  renderWiFiStatus(status, fullDetails, classes) {
    const listItems = [];

    listItems.push(
      <ListItem key="connection_status">
        <Avatar className={classes["wifiStatus_" + connectionStatusHighlight(status)]}>
          <WifiIcon />
        </Avatar>
        <ListItemText primary="Connection Status" secondary={connectionStatus(status)} />
      </ListItem>
    );

    if (fullDetails && isConnected(status)) {
      listItems.push(<Divider key="connection_status_divider" inset component="li" />);

      listItems.push(
        <ListItem key="ssid">
          <Avatar>
            <SettingsInputAntennaIcon />
          </Avatar>
          <ListItemText primary="SSID" secondary={status.ssid} />
        </ListItem>
      );
      listItems.push(<Divider key="ssid_divider" inset component="li" />);

      listItems.push(
        <ListItem key="ip_address">
          <Avatar>IP</Avatar>
          <ListItemText primary="IP Address" secondary={status.local_ip} />
        </ListItem>
      );
      listItems.push(<Divider key="ip_address_divider" inset component="li" />);

      listItems.push(
        <ListItem key="subnet_mask">
          <Avatar>#</Avatar>
          <ListItemText primary="Subnet Mask" secondary={status.subnet_mask} />
        </ListItem>
      );
      listItems.push(<Divider key="subnet_mask_divider" inset component="li" />);

      listItems.push(
        <ListItem key="gateway_ip">
          <Avatar>
            <SettingsInputComponentIcon />
          </Avatar>
          <ListItemText primary="Gateway IP" secondary={status.gateway_ip ? status.gateway_ip : "none"} />
        </ListItem>
      );
      listItems.push(<Divider key="gateway_ip_divider" inset component="li" />);

      listItems.push(
        <ListItem key="dns_server_ip">
          <Avatar>
            <DNSIcon />
          </Avatar>
          <ListItemText primary="DNS Server IP" secondary={this.dnsServers(status)} />
        </ListItem>
      );
    }

    return  (
      <div>
        <List>
        {listItems}
        </List>
        <Button variant="raised" color="secondary" className={classes.button} onClick={this.loadWiFiStatus}>
          Refresh
        </Button>
      </div>
    );

  }

  render() {
    const { status, fetched, errorMessage } = this.state;
    const { classes, fullDetails }  = this.props;

    return (
      <SectionContent title="WiFi Status">
        <SnackbarNotification notificationRef={(raiseNotification)=>this.raiseNotification = raiseNotification} />
        {
         !fetched ?
         <div>
           <LinearProgress className={classes.fetching}/>
           <Typography variant="display1" className={classes.fetching}>
             Loading...
           </Typography>
         </div>
       :
        status ? this.renderWiFiStatus(status, fullDetails, classes)
       :
        <div>
          <Typography variant="display1" className={classes.fetching}>
            {errorMessage}
          </Typography>
          <Button variant="raised" color="secondary" className={classes.button} onClick={this.loadWiFiStatus}>
            Refresh
          </Button>
        </div>
      }
      </SectionContent>
    )
  }
}

export default withStyles(styles)(WiFiStatus);
