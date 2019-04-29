import React, { Component, Fragment } from 'react';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import Avatar from '@material-ui/core/Avatar';
import Divider from '@material-ui/core/Divider';
import UpdateIcon from '@material-ui/icons/Update';
import SDStorageIcon from '@material-ui/icons/SdStorage';
import NetworkWifiIcon from '@material-ui/icons/NetworkWifi'; 
import StorageIcon from '@material-ui/icons/Storage';
import MemoryIcon from '@material-ui/icons/Memory';
import InfoIcon from '@material-ui/icons/Info';

import { ESP_STATUS_ENDPOINT }  from  '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import SectionContent from '../components/SectionContent';

const styles = theme => ({
  fetching: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class ESPStatus extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  createListItemsESP32(data, fullDetails, classes){
    return (
      <Fragment>
        <List>
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="Platform" secondary={data.esp_platform} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="Cycle Count" secondary={data.cycle_count} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="CPU Freq MHZ" secondary={data.cpu_freq_mhz} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <MemoryIcon />
            </Avatar>
            <ListItemText primary="Heap Size" secondary={data.heap_size} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <MemoryIcon />
            </Avatar>
            <ListItemText primary="Free Heap" secondary={data.free_heap} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <MemoryIcon />
            </Avatar>
            <ListItemText primary="Min Free Heap" secondary={data.min_free_heap} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <MemoryIcon />
            </Avatar>
            <ListItemText primary="Max Alloc Heap" secondary={data.max_alloc_heap} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <UpdateIcon />
            </Avatar>
            <ListItemText primary="SPI RAM Size" secondary={data.psram_size} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <UpdateIcon />
            </Avatar>
            <ListItemText primary="SPI RAM Free Size" secondary={data.free_psram} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <UpdateIcon />
            </Avatar>
            <ListItemText primary="SPI RAM Min Free Size" secondary={data.min_free_psram} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <UpdateIcon />
            </Avatar>
            <ListItemText primary="SPI RAM Max Alloc Size" secondary={data.max_alloc_psram} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <StorageIcon />
            </Avatar>
            <ListItemText primary="Sketch Size" secondary={data.sketch_size} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <StorageIcon />
            </Avatar>
            <ListItemText primary="Sketch MD5" secondary={data.sketch_md5} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <StorageIcon />
            </Avatar>
            <ListItemText primary="Free Sketch Space" secondary={data.free_sketch_space} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="Chip Revision" secondary={data.chip_revision} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="SDK Version" secondary={data.sdk_version} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <SDStorageIcon />
            </Avatar>
            <ListItemText primary="Flash Chip Size" secondary={data.flash_chip_size} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <SDStorageIcon />
            </Avatar>
            <ListItemText primary="Flash Chip Speed" secondary={data.flash_chip_speed} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <SDStorageIcon />
            </Avatar>
            <ListItemText primary="Flash Chip Mode" secondary={data.flash_chip_mode} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <NetworkWifiIcon />
            </Avatar>
            <ListItemText primary="Factory MAC Address" secondary={data.factory_mac_address} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <NetworkWifiIcon />
            </Avatar>
            <ListItemText primary="Current MAC Address" secondary={data.current_mac_address} />
          </ListItem>
        </List>
      </Fragment>
    );
  }

  createListItemsESP8266(data, fullDetails, classes){
    return (
      <Fragment>
        <List>
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="Platform" secondary={data.esp_platform} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="Cycle Count" secondary={data.cycle_count} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="CPU Freq MHZ" secondary={data.cpu_freq_mhz} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="Last Reset Reason" secondary={data.last_reset_reason} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <MemoryIcon />
            </Avatar>
            <ListItemText primary="Free Heap" secondary={data.free_heap} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <MemoryIcon />
            </Avatar>
            <ListItemText primary="Heap Fragmentation" secondary={data.heap_fragmentation} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <MemoryIcon />
            </Avatar>
            <ListItemText primary="Max Free Block Size" secondary={data.max_free_block_size} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="Chip Id" secondary={data.chip_id} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="Core Version" secondary={data.core_version} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <InfoIcon />
            </Avatar>
            <ListItemText primary="SDK Version" secondary={data.sdk_version} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <StorageIcon />
            </Avatar>
            <ListItemText primary="Sketch Size" secondary={data.sketch_size} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <StorageIcon />
            </Avatar>
            <ListItemText primary="Free Sketch Space" secondary={data.free_sketch_space} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <StorageIcon />
            </Avatar>
            <ListItemText primary="Sketch MD5" secondary={data.sketch_md5} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <SDStorageIcon />
            </Avatar>
            <ListItemText primary="Flash Chip Id" secondary={data.flash_chip_id} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <SDStorageIcon />
            </Avatar>
            <ListItemText primary="Flash Chip Size" secondary={data.flash_chip_size} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <SDStorageIcon />
            </Avatar>
            <ListItemText primary="Flash Chip Real Size" secondary={data.flash_chip_real_size} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <SDStorageIcon />
            </Avatar>
            <ListItemText primary="Flash Chip Speed" secondary={data.flash_chip_speed} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <Avatar>
              <NetworkWifiIcon />
            </Avatar>
            <ListItemText primary="Current Mac Address" secondary={data.current_mac_address} />
          </ListItem>
          <Divider variant="inset" component="li" />
        </List>
      </Fragment>
    );
  }

  renderESPStatus(data, fullDetails, classes){
    return  (
      <div>
        {data.esp_platform === "esp32" && this.createListItemsESP32(data, fullDetails, classes)}
        {data.esp_platform === "esp8266" && this.createListItemsESP8266(data, fullDetails, classes)}
        <Button variant="contained" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { data, fetched, errorMessage, classes, fullDetails }  = this.props;

    return (
      <SectionContent title="ESP Status">
        {
         !fetched ?
         <div>
           <LinearProgress className={classes.fetching}/>
           <Typography variant="display1" className={classes.fetching}>
             Loading...
           </Typography>
         </div>
       :
        data ? this.renderESPStatus(data, fullDetails, classes)
       :
        <div>
          <Typography variant="display1" className={classes.fetching}>
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

export default restComponent(ESP_STATUS_ENDPOINT, withStyles(styles)(ESPStatus));
