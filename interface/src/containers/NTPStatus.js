import React, { Component, Fragment } from 'react';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import ListItemText from '@material-ui/core/ListItemText';
import Avatar from '@material-ui/core/Avatar';
import Divider from '@material-ui/core/Divider';

import SwapVerticalCircleIcon from '@material-ui/icons/SwapVerticalCircle';
import AccessTimeIcon from '@material-ui/icons/AccessTime';
import DNSIcon from '@material-ui/icons/Dns';
import UpdateIcon from '@material-ui/icons/Update';
import AvTimerIcon from '@material-ui/icons/AvTimer';
import RefreshIcon from '@material-ui/icons/Refresh';

import { isNtpActive, ntpStatusHighlight, ntpStatus } from '../constants/NTPStatus';
import * as Highlight from '../constants/Highlight';
import { formatIsoDateTime } from '../constants/TimeFormat';
import { NTP_STATUS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';
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
        {
          isNtpActive(data) && (
            <Fragment>
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <AccessTimeIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="Local Time" secondary={formatIsoDateTime(data.time_local)} />
              </ListItem>
              <Divider variant="inset" component="li" />
              <ListItem>
                <ListItemAvatar>
                  <Avatar>
                    <AccessTimeIcon />
                  </Avatar>
                </ListItemAvatar>
                <ListItemText primary="UTC Time" secondary={formatIsoDateTime(data.time_utc)} />
              </ListItem>
              <Divider variant="inset" component="li" />
            </Fragment>
          )}
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
        <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { data, fetched, errorMessage, loadData, classes } = this.props;
    return (
      <SectionContent title="NTP Status">
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={
            () => this.renderNTPStatus(data, classes)
          }
        />
      </SectionContent>
    );
  }
}

export default restComponent(NTP_STATUS_ENDPOINT, withStyles(styles)(NTPStatus));
