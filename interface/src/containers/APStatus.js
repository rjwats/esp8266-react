import React, { Component } from 'react';

import { withStyles } from 'material-ui/styles';
import Button from 'material-ui/Button';
import { LinearProgress } from 'material-ui/Progress';
import Typography from 'material-ui/Typography';
import List, { ListItem, ListItemText } from 'material-ui/List';
import Avatar from 'material-ui/Avatar';
import Divider from 'material-ui/Divider';
import SettingsInputAntennaIcon from 'material-ui-icons/SettingsInputAntenna';
import DeviceHubIcon from 'material-ui-icons/DeviceHub';
import ComputerIcon from 'material-ui-icons/Computer';

import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent'

import * as Highlight from '../constants/Highlight';
import { AP_STATUS_ENDPOINT }  from  '../constants/Endpoints';

const styles = theme => ({
  ["apStatus_" + Highlight.SUCCESS]: {
    backgroundColor: theme.palette.highlight_success
  },
  ["apStatus_" + Highlight.IDLE]: {
    backgroundColor: theme.palette.highlight_idle
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

class APStatus extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  apStatusHighlight(data){
    return data.active ? Highlight.SUCCESS : Highlight.IDLE;
  }

  apStatus(data){
    return data.active ? "Active" : "Inactive";
  }

  // active, ip_address, mac_address, station_num

  renderAPStatus(data, fullDetails, classes){
    const listItems = [];

    listItems.push(
      <ListItem key="ap_status">
        <Avatar className={classes["apStatus_" + this.apStatusHighlight(data)]}>
          <SettingsInputAntennaIcon />
        </Avatar>
        <ListItemText primary="Status" secondary={this.apStatus(data)} />
      </ListItem>
    );
    listItems.push(<Divider key="ap_status_divider" inset component="li" />);

    listItems.push(
      <ListItem key="ip_address">
        <Avatar>IP</Avatar>
        <ListItemText primary="IP Address" secondary={data.ip_address} />
      </ListItem>
    );
    listItems.push(<Divider key="ip_address_divider" inset component="li" />);

    listItems.push(
      <ListItem key="mac_address">
        <Avatar>
          <DeviceHubIcon />
        </Avatar>
        <ListItemText primary="MAC Address" secondary={data.mac_address} />
      </ListItem>
    );
    listItems.push(<Divider key="mac_address_divider" inset component="li" />);

    listItems.push(
      <ListItem key="station_num">
        <Avatar>
          <ComputerIcon />
        </Avatar>
        <ListItemText primary="AP Clients" secondary={data.station_num} />
      </ListItem>
    );
    listItems.push(<Divider key="station_num_divider" inset component="li" />);

    return  (
      <div>
        <List>
          {listItems}
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
      <SectionContent title="AP Status">
        {
         !fetched ?
         <div>
           <LinearProgress className={classes.fetching}/>
           <Typography variant="display1" className={classes.fetching}>
             Loading...
           </Typography>
         </div>
       :
        data ? this.renderAPStatus(data, fullDetails, classes)
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

export default restComponent(AP_STATUS_ENDPOINT, withStyles(styles)(APStatus));
