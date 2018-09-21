import React, { Component, Fragment } from 'react';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';

import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';

import Avatar from '@material-ui/core/Avatar';
import Divider from '@material-ui/core/Divider';
import WifiIcon from '@material-ui/icons/Wifi';
import DNSIcon from '@material-ui/icons/Dns';
import SettingsInputComponentIcon from '@material-ui/icons/SettingsInputComponent';
import SettingsInputAntennaIcon from '@material-ui/icons/SettingsInputAntenna';

import SectionContent from '../components/SectionContent';
import { WIFI_STATUS_ENDPOINT }  from  '../constants/Endpoints';
import { isConnected, connectionStatus, connectionStatusHighlight }  from  '../constants/WiFiConnectionStatus';
import * as Highlight from '../constants/Highlight';
import { restComponent }  from  '../components/RestComponent';

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

  componentDidMount() {
    this.props.loadData();
  }

  dnsServers(status) {
    if (!status.dns_ip_1){
      return "none";
    }
    return status.dns_ip_1 + (status.dns_ip_2 ? ','+status.dns_ip_2 : '');
  }

  createListItems(data, fullDetails, classes) {
    return (
      <Fragment>
        <ListItem>
          <Avatar className={classes["wifiStatus_" + connectionStatusHighlight(data)]}>
            <WifiIcon />
          </Avatar>
          <ListItemText primary="Connection Status" secondary={connectionStatus(data)} />
        </ListItem>
        <Divider inset component="li" />
        {
          (fullDetails && isConnected(data)) &&
          <Fragment>
            <ListItem>
              <Avatar>
                <SettingsInputAntennaIcon />
              </Avatar>
              <ListItemText primary="SSID" secondary={data.ssid} />
            </ListItem>
            <Divider inset component="li" />
            <ListItem>
              <Avatar>IP</Avatar>
              <ListItemText primary="IP Address" secondary={data.local_ip} />
            </ListItem>
            <Divider inset component="li" />
            <ListItem>
              <Avatar>#</Avatar>
              <ListItemText primary="Subnet Mask" secondary={data.subnet_mask} />
            </ListItem>
            <Divider inset component="li" />
            <ListItem>
              <Avatar>
                <SettingsInputComponentIcon />
              </Avatar>
              <ListItemText primary="Gateway IP" secondary={data.gateway_ip ? data.gateway_ip : "none"} />
            </ListItem>
            <Divider inset component="li" />
            <ListItem>
              <Avatar>
                <DNSIcon />
              </Avatar>
              <ListItemText primary="DNS Server IP" secondary={this.dnsServers(data)} />
            </ListItem>
            <Divider inset component="li" />
          </Fragment>
        }
      </Fragment>
    );
  }

  renderWiFiStatus(data, fullDetails, classes) {
    return  (
      <div>
        <List>
        {this.createListItems(data, fullDetails, classes)}
        </List>
        <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );

  }

  render() {
    const { data, fetched, errorMessage, classes, fullDetails }  = this.props;

    return (
      <SectionContent title="WiFi Status">
        {
         !fetched ?
         <div>
           <LinearProgress className={classes.fetching}/>
           <Typography variant="display1" className={classes.fetching}>
             Loading...
           </Typography>
         </div>
       :
        data ? this.renderWiFiStatus(data, fullDetails, classes)
       :
        <div>
          <Typography variant="display1" className={classes.fetching}>
            {errorMessage}
          </Typography>
          <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
            Refresh
          </Button>
        </div>
      }
      </SectionContent>
    )
  }
}

export default restComponent(WIFI_STATUS_ENDPOINT, withStyles(styles)(WiFiStatus));
