import React, { Component, Fragment } from 'react';

import { Avatar, Button, Divider, Dialog, DialogTitle, DialogContent, DialogActions } from '@material-ui/core';
import { List, ListItem, ListItemAvatar, ListItemText } from '@material-ui/core';

import DevicesIcon from '@material-ui/icons/Devices';
import MemoryIcon from '@material-ui/icons/Memory';
import ShowChartIcon from '@material-ui/icons/ShowChart';
import SdStorageIcon from '@material-ui/icons/SdStorage';
import DataUsageIcon from '@material-ui/icons/DataUsage';
import AutorenewIcon from '@material-ui/icons/Autorenew';
import RefreshIcon from '@material-ui/icons/Refresh';

import { redirectingAuthorizedFetch } from '../authentication/Authentication';
import { RestFormProps, FormButton, FormActions } from '../components';
import { RESTART_ENDPOINT } from '../constants/Endpoints';

import { SystemStatus } from './SystemStatusController';

interface SystemStatusFormState {
  confirmRestart: boolean;
  processing: boolean;
}

type SystemStatusFormProps = RestFormProps<SystemStatus>;

class SystemStatusForm extends Component<SystemStatusFormProps, SystemStatusFormState> {

  state: SystemStatusFormState = {
    confirmRestart: false,
    processing: false
  }

  createListItems() {
    const { data } = this.props
    return (
      <Fragment>
        <ListItem >
          <ListItemAvatar>
            <Avatar>
              <DevicesIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Platform" secondary={data.esp_platform} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem >
          <ListItemAvatar>
            <Avatar>
              <ShowChartIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="CPU Frequency" secondary={data.cpu_freq_mhz + ' MHz'} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem >
          <ListItemAvatar>
            <Avatar>
              <MemoryIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Free Heap" secondary={data.free_heap + ' bytes'} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem >
          <ListItemAvatar>
            <Avatar>
              <DataUsageIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Sketch Size (used/max)" secondary={data.sketch_size + ' / ' + data.free_sketch_space + ' bytes'} />
        </ListItem>
        <Divider variant="inset" component="li" />
        <ListItem >
          <ListItemAvatar>
            <Avatar>
              <SdStorageIcon />
            </Avatar>
          </ListItemAvatar>
          <ListItemText primary="Flash Chip Size" secondary={data.flash_chip_size + ' bytes'} />
        </ListItem>
        <Divider variant="inset" component="li" />
      </Fragment>
    );
  }

  renderRestartDialog() {
    return (
      <Dialog
        open={this.state.confirmRestart}
        onClose={this.onRestartRejected}
      >
        <DialogTitle>Confirm Restart</DialogTitle>
        <DialogContent dividers={true}>
          Are you sure you want to restart the device?
        </DialogContent>
        <DialogActions>
          <Button startIcon={<AutorenewIcon />} variant="contained" onClick={this.onRestartConfirmed} disabled={this.state.processing} color="primary" autoFocus>
            Restart
          </Button>
          <Button variant="contained" onClick={this.onRestartRejected} color="secondary">
            Cancel
          </Button>
        </DialogActions>
      </Dialog>
    )
  }

  onRestart = () => {
    this.setState({ confirmRestart: true });
  }

  onRestartRejected = () => {
    this.setState({ confirmRestart: false });
  }

  onRestartConfirmed = () => {
    this.setState({ processing: true });
    redirectingAuthorizedFetch(RESTART_ENDPOINT, { method: 'POST' })
      .then(response => {
        if (response.status === 200) {
          this.props.enqueueSnackbar("Device is restarting", { variant: 'info' });
          this.setState({ processing: false, confirmRestart: false });
        } else {
          throw Error("Invalid status code: " + response.status);
        }
      })
      .catch(error => {
        this.props.enqueueSnackbar(error.message || "Problem restarting device", { variant: 'error' });
        this.setState({ processing: false, confirmRestart: false });
      });
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
          <FormButton startIcon={<AutorenewIcon />} variant="contained" color="primary" onClick={this.onRestart}>
            Restart
          </FormButton>
        </FormActions>
        {this.renderRestartDialog()}
      </Fragment>
    );
  }

}

export default SystemStatusForm;
