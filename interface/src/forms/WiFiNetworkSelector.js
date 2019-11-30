import React, { Component } from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';

import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';

import Avatar from '@material-ui/core/Avatar';
import Badge from '@material-ui/core/Badge';

import WifiIcon from '@material-ui/icons/Wifi';
import LockIcon from '@material-ui/icons/Lock';
import LockOpenIcon from '@material-ui/icons/LockOpen';
import PermScanWifiIcon from '@material-ui/icons/PermScanWifi';

import { isNetworkOpen, networkSecurityMode } from '../constants/WiFiSecurityModes';

const styles = theme => ({
  scanningProgress: {
    margin: theme.spacing(4),
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

class WiFiNetworkSelector extends Component {

  constructor(props) {
    super(props);

    this.renderNetwork = this.renderNetwork.bind(this);
  }

  renderNetwork(network) {
    return (
      <ListItem key={network.bssid} button onClick={() => this.props.selectNetwork(network)}>
        <ListItemAvatar>
          <Avatar>
            {isNetworkOpen(network) ? <LockOpenIcon /> : <LockIcon />}
          </Avatar>
        </ListItemAvatar>
        <ListItemText
          primary={network.ssid}
          secondary={"Security: "+ networkSecurityMode(network) + ", Ch: " + network.channel}
        />
        <ListItemIcon>
          <Badge badgeContent={network.rssi + "db"}>
            <WifiIcon />
          </Badge>
        </ListItemIcon>
      </ListItem>
    );
  }

  render() {
    const { classes, scanningForNetworks, networkList, errorMessage, requestNetworkScan } = this.props;
    return (
      <div>
        {
          scanningForNetworks ?
          <div>
            <LinearProgress className={classes.scanningProgress}/>
            <Typography variant="h6" className={classes.scanningProgress}>
              Scanning...
            </Typography>
          </div>
          :
          networkList ?
          <List>
            {networkList.networks.map(this.renderNetwork)}
          </List>
          :
          <div>
            <Typography variant="h6" className={classes.scanningProgress}>
              {errorMessage}
            </Typography>
          </div>
        }

        <Button startIcon={<PermScanWifiIcon />} variant="contained" color="secondary" className={classes.button} onClick={requestNetworkScan} disabled={scanningForNetworks}>
          Scan again...
        </Button>
      </div>
    );
  }
}

WiFiNetworkSelector.propTypes = {
  classes: PropTypes.object.isRequired,
  selectNetwork: PropTypes.func.isRequired,
  scanningForNetworks: PropTypes.bool.isRequired,
  errorMessage: PropTypes.string,
  networkList: PropTypes.object,
  requestNetworkScan: PropTypes.func.isRequired
};

export default withStyles(styles)(WiFiNetworkSelector);
