import React, { Component, Fragment } from 'react';

import { withStyles, createStyles, Theme, WithStyles } from '@material-ui/core/styles';
import { List, ListItem, ListItemAvatar, ListItemText, Divider } from '@material-ui/core';
import { Dialog, DialogTitle, DialogActions, DialogContent } from '@material-ui/core';
import { Button, Avatar } from '@material-ui/core';

import DevicesIcon from '@material-ui/icons/Devices';
import MemoryIcon from '@material-ui/icons/Memory';
import ShowChartIcon from '@material-ui/icons/ShowChart';
import SdStorageIcon from '@material-ui/icons/SdStorage';
import DataUsageIcon from '@material-ui/icons/DataUsage';
import AutorenewIcon from '@material-ui/icons/Autorenew';
import RefreshIcon from '@material-ui/icons/Refresh';

import { SYSTEM_STATUS_ENDPOINT, RESTART_ENDPOINT } from '../constants/Endpoints';

import { redirectingAuthorizedFetch } from '../authentication/Authentication';
import RestFormLoader from '../components/RestFormLoader';
import SectionContent from '../components/SectionContent';
import { restController, RestControllerProps } from '../components/RestController';

const styles = (theme: Theme) => createStyles({
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

interface SystemStatusData {
  esp_platform: string;
  cpu_freq_mhz: number;
  free_heap: number;
  sketch_size: number;
  free_sketch_space: number;
  flash_chip_size: number;
}

type SystemStatusProps = RestControllerProps<SystemStatusData> & WithStyles<typeof styles>;

interface SystemStatusState {
  confirmRestart: boolean;
  processing: boolean;
}

class SystemStatus extends Component<SystemStatusProps, SystemStatusState> {

  constructor(props: SystemStatusProps) {
    super(props);

    this.state = {
      confirmRestart: false,
      processing: false
    }
  }

  componentDidMount() {
    this.props.loadData();
  }

  createListItems(data: SystemStatusData) {
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

  renderSystemStatus(data: SystemStatusData) {
    const { classes } = this.props;
    return (
      <div>
        <List>
          {this.createListItems(data)}
        </List>
        <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
        <Button startIcon={<AutorenewIcon />} variant="contained" color="secondary" className={classes.button} onClick={this.onRestart}>
          Restart
        </Button>
        {this.renderRestartDialog()}
      </div>
    );
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

  render() {
    return (
      <SectionContent title="System Status" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps =>
            this.renderSystemStatus(formProps.data)
          }
        />
      </SectionContent>
    );
  }

}

export default restController(SYSTEM_STATUS_ENDPOINT, withStyles(styles)(SystemStatus));
