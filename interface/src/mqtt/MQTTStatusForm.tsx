import React, { Component, Fragment } from 'react';

import { WithTheme, withTheme } from '@material-ui/core/styles';
import { Avatar, Divider, List, ListItem, ListItemAvatar, ListItemText } from '@material-ui/core';

import DeviceHubIcon from '@material-ui/icons/DeviceHub';
import RefreshIcon from '@material-ui/icons/Refresh';
import ReportIcon from '@material-ui/icons/Report';

import { RestFormProps, FormActions, FormButton, HighlightAvatar } from '../components';
import { mqttStatusHighlight, mqttStatus, disconnectReason } from './MQTTStatus';
import { MQTTStatus } from './types';

type MQTTStatusFormProps = RestFormProps<MQTTStatus> & WithTheme;

class MQTTStatusForm extends Component<MQTTStatusFormProps> {

  createListItems() {
    const { data, theme } = this.props
    return (
      <Fragment>
        <ListItem>
          <ListItemAvatar>
            <HighlightAvatar color={mqttStatusHighlight(data, theme)}>
              <DeviceHubIcon />
            </HighlightAvatar>
          </ListItemAvatar>
          <ListItemText primary="Status" secondary={mqttStatus(data)} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem>
          <ListItemAvatar>
            <Avatar>#</Avatar>
          </ListItemAvatar>
          <ListItemText primary="Client ID" secondary={data.client_id} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem>
          <ListItemAvatar>
            <Avatar>
              <ReportIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Disconnect Reason" secondary={disconnectReason(data)} />
        </ListItem>
        <Divider variant="inset" component="li" />
      </Fragment>
    );
  }

  render() {
    return (
      <Fragment>
        <List>
          {this.createListItems()}
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

export default withTheme(MQTTStatusForm);
