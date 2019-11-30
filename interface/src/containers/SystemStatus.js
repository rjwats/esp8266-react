import React, { Component, Fragment } from 'react';
import { withSnackbar } from 'notistack';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import ListItemText from '@material-ui/core/ListItemText';
import Avatar from '@material-ui/core/Avatar';
import Divider from '@material-ui/core/Divider';
import Dialog from '@material-ui/core/Dialog';
import DialogActions from '@material-ui/core/DialogActions';
import DialogTitle from '@material-ui/core/DialogTitle';
import DialogContent from '@material-ui/core/DialogContent';

import DevicesIcon from '@material-ui/icons/Devices';
import MemoryIcon from '@material-ui/icons/Memory';
import ShowChartIcon from '@material-ui/icons/ShowChart';
import SdStorageIcon from '@material-ui/icons/SdStorage';
import DataUsageIcon from '@material-ui/icons/DataUsage';
import AutorenewIcon from '@material-ui/icons/Autorenew';
import RefreshIcon from '@material-ui/icons/Refresh';

import { SYSTEM_STATUS_ENDPOINT, RESET_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';
import SectionContent from '../components/SectionContent';
import { redirectingAuthorizedFetch } from '../authentication/Authentication';

const styles = theme => ({
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

class SystemStatus extends Component {


  constructor(props) {
    super(props);

    this.state = {
      confirmReset: false,
      processing: false
    }
  }

  componentDidMount() {
    this.props.loadData();
  }

  createListItems(data, classes) {
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

  renderSystemStatus(data, classes) {
    return (
      <div>
        <List>
          {this.createListItems(data, classes)}
        </List>
        <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
        <Button startIcon={<AutorenewIcon />} variant="contained" color="secondary" className={classes.button} onClick={this.onReset}>
          Reset
        </Button>
      </div>
    );
  }

  onReset = () => {
    this.setState({ confirmReset: true });
  }

  onResetRejected = () => {
    this.setState({ confirmReset: false });
  }

  onResetConfirmed = () => {
    this.setState({ processing: true });
    redirectingAuthorizedFetch(RESET_ENDPOINT, { method: 'POST' })
      .then(response => {
        if (response.status === 200) {
          this.props.enqueueSnackbar("Device is resetting", { variant: 'info' });
          this.setState({ processing: false, confirmReset: false });
        } else {
          throw Error("Invalid status code: " + response.status);
        }
      })
      .catch(error => {
        this.props.enqueueSnackbar(error.message || "Problem resetting device", { variant: 'error' });
        this.setState({ processing: false, confirmReset: false });
      });
  }

  renderResetDialog() {
    return (
      <Dialog
        open={this.state.confirmReset}
        onClose={this.onResetRejected}
      >
        <DialogTitle>Confirm Reset</DialogTitle>
        <DialogContent dividers={true}>
          Are you sure you want to reset the device?
        </DialogContent>
        <DialogActions>
          <Button startIcon={<AutorenewIcon />} variant="contained" onClick={this.onResetConfirmed} disabled={this.state.processing} color="primary" autoFocus>
            Reset
          </Button>
          <Button variant="contained" onClick={this.onResetRejected} color="secondary">
            Cancel
          </Button>
        </DialogActions>
      </Dialog>
    )
  }

  render() {
    const { data, fetched, errorMessage, loadData, classes } = this.props;
    return (
      <SectionContent title="System Status">
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={
            () => this.renderSystemStatus(data, classes)
          }
        />
        {this.renderResetDialog()}
      </SectionContent>
    )
  }

}

export default withSnackbar(restComponent(SYSTEM_STATUS_ENDPOINT, withStyles(styles)(SystemStatus)));
