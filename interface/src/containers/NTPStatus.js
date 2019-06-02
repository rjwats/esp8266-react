import React, { Component, Fragment } from 'react';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import ListItemText from '@material-ui/core/ListItemText';
import Avatar from '@material-ui/core/Avatar';
import Divider from '@material-ui/core/Divider';
import SwapVerticalCircleIcon from '@material-ui/icons/SwapVerticalCircle';
import AccessTimeIcon from '@material-ui/icons/AccessTime';
import DNSIcon from '@material-ui/icons/Dns';
import TimerIcon from '@material-ui/icons/Timer';
import UpdateIcon from '@material-ui/icons/Update';
import AvTimerIcon from '@material-ui/icons/AvTimer';

import { isSynchronized, ntpStatusHighlight, ntpStatus } from '../constants/NTPStatus';
import * as Highlight from '../constants/Highlight';
import { unixTimeToTimeAndDate } from '../constants/TimeFormat';
import { NTP_STATUS_ENDPOINT } from '../constants/Endpoints';
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
    margin: theme.spacing(4),
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

class NTPStatus extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  createListItems(data, classes) {
    return (
      <Fragment>
        <ListItem >
          <ListItemAvatar>
            <Avatar className={classes["ntpStatus_" + ntpStatusHighlight(data)]}>
              <UpdateIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Status" secondary={ntpStatus(data)} />
        </ListItem>
        <Divider variant="inset" component="li" />
        {isSynchronized(data) &&
          <Fragment>
            <ListItem>
              <ListItemAvatar>
                <Avatar>
                  <AccessTimeIcon />
                </Avatar>
              </ListItemAvatar>
              <ListItemText primary="Time Now" secondary={unixTimeToTimeAndDate(data.now)} />
            </ListItem>
            <Divider variant="inset" component="li" />
            <ListItem>
              <ListItemAvatar>
                <Avatar>
                  <SwapVerticalCircleIcon />
                </Avatar>
              </ListItemAvatar>
              <ListItemText primary="Last Sync" secondary={data.last_sync > 0 ? unixTimeToTimeAndDate(data.last_sync) : "never"} />
            </ListItem>
            <Divider variant="inset" component="li" />
          </Fragment>
        }
        <ListItem>
          <ListItemAvatar>
            <Avatar>
              <DNSIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="NTP Server" secondary={data.server} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem>
          <ListItemAvatar>
            <Avatar>
              <TimerIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Sync Interval" secondary={moment.duration(data.interval, 'seconds').humanize()} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem>
          <ListItemAvatar>
            <Avatar>
              <AvTimerIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Uptime" secondary={moment.duration(data.uptime, 'seconds').humanize()} />
        </ListItem>
        <Divider variant="inset" component="li" />
      </Fragment>
    );
  }

  renderNTPStatus(data, classes) {
    return (
      <div>
        <List>
          {this.createListItems(data, classes)}
        </List>
        <Button variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { data, fetched, errorMessage, classes } = this.props;

    return (
      <SectionContent title="NTP Status">
        {
          !fetched ?
            <div>
              <LinearProgress className={classes.fetching} />
              <Typography variant="h4" className={classes.fetching}>
                Loading...
              </Typography>
            </div>
            :
            data ? this.renderNTPStatus(data, classes)
              :
              <div>
                <Typography variant="h4" className={classes.fetching}>
                  {errorMessage}
                </Typography>
                <Button variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
                  Refresh
                </Button>
              </div>
        }
      </SectionContent>
    )
  }
}

export default restComponent(NTP_STATUS_ENDPOINT, withStyles(styles)(NTPStatus));
