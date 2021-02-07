import React, { Component, Fragment } from 'react';

import { WithTheme, withTheme } from '@material-ui/core/styles';
import { Avatar, Divider, List, ListItem, ListItemAvatar, ListItemText, Switch} from '@material-ui/core';

import DeviceHubIcon from '@material-ui/icons/DeviceHub';
import RefreshIcon from '@material-ui/icons/Refresh';
import ReportIcon from '@material-ui/icons/Report';

import { RestFormProps, FormActions, FormButton, HighlightAvatar } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';
import { serialStatusHighlight, serialStatus  } from './SerialStatus';
import { SerialStatus, Config } from './types';

type SerialStatusFormProps = RestFormProps<SerialStatus> & WithTheme;

class SerialStatusForm extends Component<SerialStatusFormProps> {
  renderConnectionStatus() {
    const { data, saveData, setData } = this.props;

    const changeLedOn = (event: React.ChangeEvent<HTMLInputElement>) => {
    }
    if (data.enabled) {
      return (
        <Fragment>
          <ListItem>
            <ListItemAvatar>
              <Avatar>#</Avatar>
            </ListItemAvatar>
            <ListItemText primary="Baud rate" secondary={Config[data.baud]} />
          </ListItem>
          <ListItem>
            <ListItemAvatar>
              <Avatar>#</Avatar>
            </ListItemAvatar>
            <ListItemText primary="Config" secondary={data.config} />
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
        </ListItem>client_id
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
            <HighlightAvatar color={serialStatusHighlight(data, theme)}>
              <DeviceHubIcon />
            </HighlightAvatar>
          </ListItemAvatar>
          <ListItemText primary="Status" secondary={serialStatus(data)} />
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

export default withTheme(SerialStatusForm);
