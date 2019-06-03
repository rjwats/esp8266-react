import React, { Component, Fragment } from 'react';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import ListItemText from '@material-ui/core/ListItemText';
import Avatar from '@material-ui/core/Avatar';
import Divider from '@material-ui/core/Divider';
import DevicesIcon from '@material-ui/icons/Devices';
import MemoryIcon from '@material-ui/icons/Memory';
import ShowChartIcon from '@material-ui/icons/ShowChart';
import SdStorageIcon from '@material-ui/icons/SdStorage';
import DataUsageIcon from '@material-ui/icons/DataUsage';


import { SYSTEM_STATUS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import SectionContent from '../components/SectionContent';

const styles = theme => ({
  fetching: {
    margin: theme.spacing(4),
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

class SystemStatus extends Component {

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

  renderNTPStatus(data, classes) {
    return (
      <div>
        <List>
          {this.createListItems(data, classes)}
        </List>
        <Button variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { data, fetched, errorMessage, classes } = this.props;
    return (
      <SectionContent title="System Status">
        {
          !fetched ?
            <div>
              <LinearProgress className={classes.fetching} />
              <Typography variant="h4" className={classes.fetching}>
                Loading...
              </Typography>
            </div>
            :
            data ? this.renderNTPStatus(data, classes)
              :
              <div>
                <Typography variant="h4" className={classes.fetching}>
                  {errorMessage}
                </Typography>
                <Button variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
                  Refresh
                </Button>
              </div>
        }
      </SectionContent>
    )
  }
}

export default restComponent(SYSTEM_STATUS_ENDPOINT, withStyles(styles)(SystemStatus));
