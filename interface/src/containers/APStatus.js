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
import SettingsInputAntennaIcon from '@material-ui/icons/SettingsInputAntenna';
import DeviceHubIcon from '@material-ui/icons/DeviceHub';
import ComputerIcon from '@material-ui/icons/Computer';

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

  createListItems(data, fullDetails, classes){
    return (
      <Fragment>
        <ListItem>
          <Avatar className={classes["apStatus_" + this.apStatusHighlight(data)]}>
            <SettingsInputAntennaIcon />
          </Avatar>
          <ListItemText primary="Status" secondary={this.apStatus(data)} />
        </ListItem>
        <Divider inset component="li" />
        <ListItem>
          <Avatar>IP</Avatar>
          <ListItemText primary="IP Address" secondary={data.ip_address} />
        </ListItem>
        <Divider inset component="li" />
        <ListItem>
          <Avatar>
            <DeviceHubIcon />
          </Avatar>
          <ListItemText primary="MAC Address" secondary={data.mac_address} />
        </ListItem>
        <Divider inset component="li" />
        <ListItem>
          <Avatar>
            <ComputerIcon />
          </Avatar>
          <ListItemText primary="AP Clients" secondary={data.station_num} />
        </ListItem>
        <Divider inset component="li" />
      </Fragment>
    );
  }

  renderAPStatus(data, fullDetails, classes){
    return  (
      <div>
        <List>
          <Fragment>
            {this.createListItems(data, fullDetails, classes)}
          </Fragment>
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
