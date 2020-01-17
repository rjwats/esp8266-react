import React, { Component, Fragment } from 'react';

import { withStyles, createStyles, Theme, WithStyles } from '@material-ui/core/styles';
import { Avatar, Divider, List, ListItem, ListItemAvatar, ListItemText } from '@material-ui/core';

import SettingsInputAntennaIcon from '@material-ui/icons/SettingsInputAntenna';
import DeviceHubIcon from '@material-ui/icons/DeviceHub';
import ComputerIcon from '@material-ui/icons/Computer';
import RefreshIcon from '@material-ui/icons/Refresh';

import { RestFormProps } from '../components/RestFormLoader';
import { APStatusData } from '../containers/APStatus';
import { apStatusHighlight, apStatus } from '../constants/APStatus';
import FormActions from '../components/FormActions';
import FormButton from '../components/FormButton';

const styles = (theme: Theme) => createStyles({
  apStatusHighlight: ({ data }: RestFormProps<APStatusData>) => ({
    backgroundColor: apStatusHighlight(data, theme)
  })
});

type APStatusFormProps = RestFormProps<APStatusData> & WithStyles<typeof styles>;

class APStatusForm extends Component<APStatusFormProps> {

  createListItems() {
    const { classes, data } = this.props
    return (
      <Fragment>
        <ListItem>
          <ListItemAvatar>
            <Avatar className={classes.apStatusHighlight}>
              <SettingsInputAntennaIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Status" secondary={apStatus(data)} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem>
          <ListItemAvatar>
            <Avatar>IP</Avatar>
          </ListItemAvatar>
          <ListItemText primary="IP Address" secondary={data.ip_address} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem>
          <ListItemAvatar>
            <Avatar>
              <DeviceHubIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="MAC Address" secondary={data.mac_address} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem>
          <ListItemAvatar>
            <Avatar>
              <ComputerIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="AP Clients" secondary={data.station_num} />
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

export default withStyles(styles)(APStatusForm);
