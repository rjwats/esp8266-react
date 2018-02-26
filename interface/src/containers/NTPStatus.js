import React, { Component } from 'react';

import { withStyles } from 'material-ui/styles';
import Button from 'material-ui/Button';
import { LinearProgress } from 'material-ui/Progress';
import Typography from 'material-ui/Typography';
import List, { ListItem, ListItemText } from 'material-ui/List';
import Avatar from 'material-ui/Avatar';
import Divider from 'material-ui/Divider';
import SwapVerticalCircleIcon from 'material-ui-icons/SwapVerticalCircle';
import AccessTimeIcon from 'material-ui-icons/AccessTime';
import DNSIcon from 'material-ui-icons/Dns';
import TimerIcon from 'material-ui-icons/Timer';
import UpdateIcon from 'material-ui-icons/Update';
import AvTimerIcon from 'material-ui-icons/AvTimer';

import { isSynchronized, ntpStatusHighlight, ntpStatus }  from  '../constants/NTPStatus';
import * as Highlight from '../constants/Highlight';
import { unixTimeToTimeAndDate } from '../constants/TimeFormat';
import { NTP_STATUS_ENDPOINT }  from  '../constants/Endpoints';

import SnackbarNotification from '../components/SnackbarNotification';
import SectionContent from '../components/SectionContent';

import { simpleGet }  from  '../helpers/SimpleGet';

import moment from 'moment';

const styles = theme => ({
  ["ntpStatus_" + Highlight.SUCCESS]: {
    backgroundColor: theme.palette.highlight_success
  },
  ["ntpStatus_" + Highlight.ERROR]: {
    backgroundColor: theme.palette.highlight_error
  },
  ["ntpStatus_" + Highlight.WARN]: {
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

class NTPStatus extends Component {

  constructor(props) {
    super(props);

    this.state = {
             status:null,
             fetched: false,
             errorMessage:null
           };

    this.setState = this.setState.bind(this);
    this.loadNTPStatus = this.loadNTPStatus.bind(this);
    this.raiseNotification=this.raiseNotification.bind(this);
  }

  componentDidMount() {
    this.loadNTPStatus();
  }

  loadNTPStatus() {
    simpleGet(
      NTP_STATUS_ENDPOINT,
      this.setState,
      this.raiseNotification
    );
  }

  raiseNotification(errorMessage) {
    this.snackbarNotification(errorMessage);
  }

  renderNTPStatus(status, fullDetails, classes){
    const listItems = [];

    listItems.push(
      <ListItem key="ntp_status">
        <Avatar className={classes["ntpStatus_" + ntpStatusHighlight(status)]}>
          <UpdateIcon />
        </Avatar>
        <ListItemText primary="Status" secondary={ntpStatus(status)} />
      </ListItem>
    );
    listItems.push(<Divider key="ntp_status_divider" inset component="li" />);

    if (isSynchronized(status)) {
      listItems.push(
        <ListItem key="time_now">
          <Avatar>
            <AccessTimeIcon />
          </Avatar>
          <ListItemText primary="Time Now" secondary={unixTimeToTimeAndDate(status.now)} />
        </ListItem>
      );
      listItems.push(<Divider key="time_now_divider" inset component="li" />);
    }

    listItems.push(
      <ListItem key="last_sync">
        <Avatar>
          <SwapVerticalCircleIcon />
        </Avatar>
        <ListItemText primary="Last Sync" secondary={status.last_sync > 0 ? unixTimeToTimeAndDate(status.last_sync) : "never" } />
      </ListItem>
    );
    listItems.push(<Divider key="last_sync_divider" inset component="li" />);

    listItems.push(
      <ListItem key="ntp_server">
        <Avatar>
          <DNSIcon />
        </Avatar>
        <ListItemText primary="NTP Server" secondary={status.server} />
      </ListItem>
    );
    listItems.push(<Divider key="ntp_server_divider" inset component="li" />);

    listItems.push(
      <ListItem key="sync_interval">
        <Avatar>
          <TimerIcon />
        </Avatar>
        <ListItemText primary="Sync Interval" secondary={moment.duration(status.interval, 'seconds').humanize()} />
      </ListItem>
    );
    listItems.push(<Divider key="sync_interval_divider" inset component="li" />);

    listItems.push(
      <ListItem key="uptime">
        <Avatar>
          <AvTimerIcon />
        </Avatar>
        <ListItemText primary="Uptime" secondary={moment.duration(status.uptime, 'seconds').humanize()} />
      </ListItem>
    );

    return  (
      <div>
        <List>
          {listItems}
        </List>
        <Button variant="raised" color="secondary" className={classes.button} onClick={this.loadNTPStatus}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { status, fetched, errorMessage } = this.state;
    const { classes, fullDetails }  = this.props;

    return (
      <SectionContent title="NTP Status">
        <SnackbarNotification notificationRef={(snackbarNotification)=>this.snackbarNotification = snackbarNotification} />
        {
         !fetched ?
         <div>
           <LinearProgress className={classes.fetching}/>
           <Typography variant="display1" className={classes.fetching}>
             Loading...
           </Typography>
         </div>
       :
        status ? this.renderNTPStatus(status, fullDetails, classes)
       :
        <div>
          <Typography variant="display1" className={classes.fetching}>
            {errorMessage}
          </Typography>
          <Button variant="raised" color="secondary" className={classes.button} onClick={this.loadNTPStatus}>
            Refresh
          </Button>
        </div>
      }
      </SectionContent>
    )
  }
}

export default withStyles(styles)(NTPStatus);
