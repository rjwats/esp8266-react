import React, { Component, Fragment } from 'react';
import moment from 'moment';

import { WithTheme, withTheme } from '@material-ui/core/styles';
import { Avatar, Divider, List, ListItem, ListItemAvatar, ListItemText } from '@material-ui/core';

import SwapVerticalCircleIcon from '@material-ui/icons/SwapVerticalCircle';
import AccessTimeIcon from '@material-ui/icons/AccessTime';
import DNSIcon from '@material-ui/icons/Dns';
import UpdateIcon from '@material-ui/icons/Update';
import AvTimerIcon from '@material-ui/icons/AvTimer';
import RefreshIcon from '@material-ui/icons/Refresh';

import { RestFormProps, FormActions, FormButton, HighlightAvatar } from '../components';

import { isNtpActive, ntpStatusHighlight, ntpStatus } from './NTPStatus';
import { NTPStatus } from './NTPStatusController';
import { formatIsoDateTime } from './TimeFormat';

type NTPStatusFormProps = RestFormProps<NTPStatus> & WithTheme;

class NTPStatusForm extends Component<NTPStatusFormProps> {

  render() {
    const { data, theme } = this.props
    return (
      <Fragment>
        <List>
          <ListItem>
            <ListItemAvatar>
              <HighlightAvatar color={ntpStatusHighlight(data, theme)}>
                <UpdateIcon />
              </HighlightAvatar>
            </ListItemAvatar>
            <ListItemText primary="Status" secondary={ntpStatus(data)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          {isNtpActive(data) && (
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
                    <SwapVerticalCircleIcon />
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
