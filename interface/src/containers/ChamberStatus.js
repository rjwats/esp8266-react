import React, { Component } from 'react';

import { withStyles } from 'material-ui/styles';
import Button from 'material-ui/Button';
import { LinearProgress } from 'material-ui/Progress';
import Typography from 'material-ui/Typography';
import List, { ListItem, ListItemText } from 'material-ui/List';
import Avatar from 'material-ui/Avatar';
import Divider from 'material-ui/Divider';
import KitchenIcon from 'material-ui-icons/Kitchen';
import NaturePeopleIcon from 'material-ui-icons/NaturePeople';
import AcUnitIcon from 'material-ui-icons/AcUnit';
import TagFacesIcon from 'material-ui-icons/TagFaces';
import WbSunnyIcon from 'material-ui-icons/WbSunny';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent'

import { chamberStatus, chamberStatusHighlight }  from  '../constants/ChamberStatus';

import * as Highlight from '../constants/Highlight';
import { CHAMBER_STATUS_ENDPOINT }  from  '../constants/Endpoints';

const styles = theme => ({
  deviceEnabled: {
    backgroundColor: theme.palette.highlight_success
  },
  ["chamberStatus_" + Highlight.IDLE]: {
    backgroundColor: theme.palette.highlight_idle
  },
  ["chamberStatus_" + Highlight.INFO]: {
    backgroundColor: theme.palette.highlight_info
  },
  ["chamberStatus_" + Highlight.ERROR]: {
    backgroundColor: theme.palette.highlight_error
  },
  fetching: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class ChamberStatus extends Component {
  
  componentDidMount() {
      this.props.loadData();
  }

  apStatusHighlight(data){
    return data.active ? Highlight.SUCCESS : Highlight.IDLE;
  }

  apStatus(data){
    return data.active ? "Active" : "Inactive";
  }

  sensorTemp(sensorId, sensors){
    if (sensors[sensorId]){
      return sensors[sensorId].temp_c.toFixed(1) + "°C";
    }
    return 'Unknown'
  }

  renderChamberStatus(data, classes){
    const {ambient_sensor_address, chamber_sensor_address, target_temp, enable_cooler, enable_heater, sensors} = data;
    return  (
      <div>
        <List>
          <ListItem>
            <Avatar className={classes["chamberStatus_" + chamberStatusHighlight(data)]}>
              <KitchenIcon />
            </Avatar>
            <ListItemText primary="Chamber Status" secondary={chamberStatus(data)} />
          </ListItem>
          <ListItem>
            <Avatar>
              <TagFacesIcon />
            </Avatar>
            <ListItemText primary="Target Temperature" secondary={target_temp + "°C"} />
          </ListItem>
          <ListItem>
            <Avatar>
              <KitchenIcon />
            </Avatar>
            <ListItemText primary="Chamber Temperature" secondary={this.sensorTemp(chamber_sensor_address, sensors)} />
          </ListItem>
          <ListItem>
            <Avatar>
              <NaturePeopleIcon />
            </Avatar>
            <ListItemText primary="Ambient Temperature" secondary={this.sensorTemp(ambient_sensor_address, sensors)} />
          </ListItem>
          <Divider inset component="li" />
          <ListItem>
            <Avatar className={enable_cooler ? classes['deviceEnabled'] : ''}>
              <AcUnitIcon />
            </Avatar>
            <ListItemText primary="Cooler Enabled?" secondary={enable_cooler ? "Enabled" : "Disabled"} />
          </ListItem>
          <ListItem>
            <Avatar className={enable_heater ? classes['deviceEnabled'] : ''}>
              <WbSunnyIcon />
            </Avatar>
            <ListItemText primary="Heater Enabled?" secondary={enable_heater ? "Enabled" : "Disabled"} />
          </ListItem>
        </List>
        <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { data, fetched, errorMessage, classes }  = this.props;

    return (
      <SectionContent title="Chamber Status">
        {
         !fetched ?
         <div>
           <LinearProgress className={classes.fetching}/>
           <Typography variant="display1" className={classes.fetching}>
             Loading...
           </Typography>
         </div>
       :
        data ? this.renderChamberStatus(data, classes)
       :
        <div>
          <Typography variant="display1" className={classes.fetching}>
            {errorMessage}
          </Typography>
          <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
            Refresh
          </Button>
        </div>
      }
      </SectionContent>
    )
  }
}

export default restComponent(CHAMBER_STATUS_ENDPOINT, withStyles(styles)(ChamberStatus));
