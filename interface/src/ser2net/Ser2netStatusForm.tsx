import React, { Component, Fragment } from 'react';

import { WithTheme, withTheme } from '@material-ui/core/styles';
import { Avatar, Divider, List, ListItem, ListItemAvatar, ListItemText, Switch} from '@material-ui/core';

import DeviceHubIcon from '@material-ui/icons/DeviceHub';
import RefreshIcon from '@material-ui/icons/Refresh';
import ReportIcon from '@material-ui/icons/Report';

import { RestFormProps, FormActions, FormButton, HighlightAvatar } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';
import { ser2netStatusHighlight, ser2netStatus, disconnectReason } from './Ser2netStatus';
import { Ser2netStatus } from './types';

type Ser2netStatusFormProps = RestFormProps<Ser2netStatus> & WithTheme;

class Ser2netStatusForm extends Component<Ser2netStatusFormProps> {
  renderConnectionStatus() {
    const { data, saveData, setData } = this.props;

    const changeLedOn = (event: React.ChangeEvent<HTMLInputElement>) => {
    }
    if (data.connected) {
      return (
        <Fragment>
          <ListItem>
            <ListItemAvatar>
              <Avatar>#</Avatar>
            </ListItemAvatar>
            <ListItemText primary="Client ID" secondary={data.client_id} />
          </ListItem>
          <Divider variant="inset" component="li" />
        </Fragment>
      );
    }
    return (
      <Fragment>
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

  createListItems() {
    const { data, theme, saveData, setData } = this.props;

    const changeLedOn = (event: React.ChangeEvent<HTMLInputElement>) => {
    }
    return (
      <Fragment>
        <ListItem>
          <ListItemAvatar>
            <HighlightAvatar color={ser2netStatusHighlight(data, theme)}>
              <DeviceHubIcon />
            </HighlightAvatar>
          </ListItemAvatar>
          <ListItemText primary="Status" secondary={ser2netStatus(data)} />
        </ListItem>
        <Divider variant="inset" component="li" />
        {data.enabled && this.renderConnectionStatus()}
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

export default withTheme(Ser2netStatusForm);
