import React, { Component, Fragment } from 'react';

import { WithTheme, withTheme } from '@material-ui/core/styles';
import { Avatar, Divider, List, ListItem, ListItemAvatar, ListItemText } from '@material-ui/core';

import SwapVerticalCircleIcon from '@material-ui/icons/SwapVerticalCircle';
import AccessTimeIcon from '@material-ui/icons/AccessTime';
import DNSIcon from '@material-ui/icons/Dns';
import TimerIcon from '@material-ui/icons/Timer';
import UpdateIcon from '@material-ui/icons/Update';
import AvTimerIcon from '@material-ui/icons/AvTimer';
import RefreshIcon from '@material-ui/icons/Refresh';

import { isSynchronized, ntpStatusHighlight, ntpStatus } from '../constants/NTPStatus';
import { unixTimeToTimeAndDate, humanReadableSeconds } from '../constants/TimeFormat';
import { RestFormProps } from '../components/RestFormLoader';
import { NTPStatusData } from '../containers/NTPStatus';
import FormActions from '../components/FormActions';
import FormButton from '../components/FormButton';
import HighlightAvatar from '../components/HighlightAvatar';

type NTPStatusFormProps = RestFormProps<NTPStatusData> & WithTheme;

class NTPStatusForm extends Component<NTPStatusFormProps> {

  render() {
    const { data, theme } = this.props
    return (
      <Fragment>
        <List>
          <ListItem >
            <ListItemAvatar>
              <HighlightAvatar color={ntpStatusHighlight(data, theme)}>
                <UpdateIcon />
              </HighlightAvatar>
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
            <ListItemText primary="Sync Interval" secondary={humanReadableSeconds(data.interval)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <Avatar>
                <AvTimerIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Uptime" secondary={humanReadableSeconds(data.uptime)} />
          </ListItem>
          <Divider variant="inset" component="li" />
        </List>
        <FormActions>
          <FormButton startIcon={<RefreshIcon />} variant="contained" color="secondary" onClick={this.props.loadData}>
            Refresh
          </FormButton>
        </FormActions>
      </Fragment>
    );
  }
}

export default withTheme(NTPStatusForm);
