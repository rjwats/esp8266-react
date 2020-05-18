import React, { Component, Fragment } from 'react';

import { Avatar, Button, Divider, Dialog, DialogTitle, DialogContent, DialogActions } from '@material-ui/core';
import { List, ListItem, ListItemAvatar, ListItemText, Typography } from '@material-ui/core';

import DevicesIcon from '@material-ui/icons/Devices';
import MemoryIcon from '@material-ui/icons/Memory';
import ShowChartIcon from '@material-ui/icons/ShowChart';
import SdStorageIcon from '@material-ui/icons/SdStorage';
import DataUsageIcon from '@material-ui/icons/DataUsage';
import PowerSettingsNewIcon from '@material-ui/icons/PowerSettingsNew';
import RefreshIcon from '@material-ui/icons/Refresh';
import SettingsBackupRestoreIcon from '@material-ui/icons/SettingsBackupRestore';
import { withStyles, createStyles, Theme, WithStyles } from '@material-ui/core/styles';

import { redirectingAuthorizedFetch } from '../authentication';
import { RestFormProps, FormButton, FormActions } from '../components';
import { FACTORY_RESET_ENDPOINT, RESTART_ENDPOINT } from '../api';

import { SystemStatus } from './types';

const styles = (theme: Theme) => createStyles({
  factoryResetButton: {
    float: "right",
    backgroundColor: theme.palette.error.light,
    '&:hover': {
      backgroundColor: theme.palette.error.dark,
    }
  },
});

interface SystemStatusFormState {
  confirmRestart: boolean;
  confirmFactoryReset: boolean;
  processing: boolean;
}

type SystemStatusFormProps = RestFormProps<SystemStatus> & WithStyles<typeof styles>;

class SystemStatusForm extends Component<SystemStatusFormProps, SystemStatusFormState> {

  state: SystemStatusFormState = {
    confirmRestart: false,
    confirmFactoryReset: false,
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
        <DialogContent dividers>
          Are you sure you want to restart the device?
        </DialogContent>
        <DialogActions>
          <Button startIcon={<PowerSettingsNewIcon />} variant="contained" onClick={this.onRestartConfirmed} disabled={this.state.processing} color="primary" autoFocus>
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

  renderFactoryResetDialog() {
    const { classes } = this.props;
    return (
      <Dialog
        open={this.state.confirmFactoryReset}
        onClose={this.onFactoryResetRejected}
      >
        <DialogTitle>Confirm Factory Reset</DialogTitle>
        <DialogContent dividers>
          All configurations will be reset to default. Device will reboot. <br/>
          This action irreversible and can not be canceled after it is confirmed. <br/>
          After reset is completed you will be able to connect to the device and set all settings from scratch. <br/>
          <Typography color="error">Are you sure you want to perform factory reset?</Typography>
        </DialogContent>
        <DialogActions>
          <Button startIcon={<SettingsBackupRestoreIcon />} variant="contained" onClick={this.onFactoryResetConfirmed} disabled={this.state.processing} className={classes.factoryResetButton} autoFocus>
            Factory Reset
          </Button>
          <Button variant="contained" onClick={this.onFactoryResetRejected} color="secondary">
            Cancel
          </Button>
        </DialogActions>
      </Dialog>
    )
  }

  onFactoryReset = () => {
    this.setState({ confirmFactoryReset: true });
  }

  onFactoryResetRejected = () => {
    this.setState({ confirmFactoryReset: false });
  }

  onFactoryResetConfirmed = () => {
    this.setState({ processing: true });
    redirectingAuthorizedFetch(FACTORY_RESET_ENDPOINT, { method: 'POST' })
      .then(response => {
        if (response.status === 200) {
          this.props.enqueueSnackbar("Factory reset is in process. It is good idea to close browser window.", { variant: 'warning' });
          this.setState({ processing: false, confirmFactoryReset: false });
        } else {
          throw Error("Invalid status code: " + response.status);
        }
      })
      .catch(error => {
        this.props.enqueueSnackbar(error.message || "Problem factory resetting device", { variant: 'error' });
        this.setState({ processing: false, confirmRestart: false });
      });
  }

  render() {
    const { classes } = this.props;
    return (
      <Fragment>
        <List>
          {this.createListItems()}
        </List>
        <FormActions>
          <FormButton startIcon={<RefreshIcon />} variant="contained" color="secondary" onClick={this.props.loadData}>
            Refresh
          </FormButton>
          <FormButton startIcon={<PowerSettingsNewIcon />} variant="contained" color="primary" onClick={this.onRestart}>
            Restart
          </FormButton>
          <FormButton startIcon={<SettingsBackupRestoreIcon />} variant="contained" className={classes.factoryResetButton} onClick={this.onFactoryReset}>
            Factory reset
          </FormButton>          
        </FormActions>
        {this.renderRestartDialog()}
        {this.renderFactoryResetDialog()}
      </Fragment>
    );
  }

}

export default withStyles(styles)(SystemStatusForm);
