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
import SwapVerticalCircleIcon from '@material-ui/icons/SwapVerticalCircle';
import AccessTimeIcon from '@material-ui/icons/AccessTime';
import DNSIcon from '@material-ui/icons/Dns';
import TimerIcon from '@material-ui/icons/Timer';
import UpdateIcon from '@material-ui/icons/Update';
import AvTimerIcon from '@material-ui/icons/AvTimer';

import { isSynchronized, ntpStatusHighlight, ntpStatus }  from  '../constants/NTPStatus';
import * as Highlight from '../constants/Highlight';
import { unixTimeToTimeAndDate } from '../constants/TimeFormat';
import { NTP_STATUS_ENDPOINT }  from  '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import SectionContent from '../components/SectionContent';

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

  componentDidMount() {
    this.props.loadData();
  }

  createListItems(data, fullDetails, classes){
    return (
      <Fragment>
        <ListItem>
          <Avatar className={classes["ntpStatus_" + ntpStatusHighlight(data)]}>
            <UpdateIcon />
          </Avatar>
          <ListItemText primary="Status" secondary={ntpStatus(data)} />
        </ListItem>
        <Divider inset component="li" />
        { isSynchronized(data) &&
          <Fragment>
            <ListItem>
              <Avatar>
                <AccessTimeIcon />
              </Avatar>
              <ListItemText primary="Time Now" secondary={unixTimeToTimeAndDate(data.now)} />
            </ListItem>
            <Divider inset component="li" />
            <ListItem>
              <Avatar>
                <SwapVerticalCircleIcon />
              </Avatar>
              <ListItemText primary="Last Sync" secondary={data.last_sync > 0 ? unixTimeToTimeAndDate(data.last_sync) : "never" } />
            </ListItem>
            <Divider inset component="li" />
          </Fragment>
        }
        <ListItem>
          <Avatar>
            <DNSIcon />
          </Avatar>
          <ListItemText primary="NTP Server" secondary={data.server} />
        </ListItem>
        <Divider inset component="li" />
        <ListItem>
          <Avatar>
            <TimerIcon />
          </Avatar>
          <ListItemText primary="Sync Interval" secondary={moment.duration(data.interval, 'seconds').humanize()} />
        </ListItem>
        <Divider inset component="li" />
        <ListItem>
          <Avatar>
            <AvTimerIcon />
          </Avatar>
          <ListItemText primary="Uptime" secondary={moment.duration(data.uptime, 'seconds').humanize()} />
        </ListItem>
        <Divider inset component="li" />
      </Fragment>
    );
  }

  renderNTPStatus(data, fullDetails, classes){
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
      <SectionContent title="NTP Status">
        {
         !fetched ?
         <div>
           <LinearProgress className={classes.fetching}/>
           <Typography variant="display1" className={classes.fetching}>
             Loading...
           </Typography>
         </div>
       :
        data ? this.renderNTPStatus(data, fullDetails, classes)
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

export default restComponent(NTP_STATUS_ENDPOINT, withStyles(styles)(NTPStatus));
