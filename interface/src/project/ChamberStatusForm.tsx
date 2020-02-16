import React, { Component, Fragment } from 'react';

import { Divider, List, Avatar, ListItemText, ListItem, withTheme, WithTheme } from '@material-ui/core';

import KitchenIcon from '@material-ui/icons/Kitchen';
import NaturePeopleIcon from '@material-ui/icons/NaturePeople';
import AcUnitIcon from '@material-ui/icons/AcUnit';
import TagFacesIcon from '@material-ui/icons/TagFaces';
import WbSunnyIcon from '@material-ui/icons/WbSunny';

import { RestFormProps, HighlightAvatar, FormActions, FormButton } from '../components';

import { ChamberStatus, SensorTemperatures } from './types';
import { chamberStatusHighlight, chamberStatus, chamberEnabledHighlight } from './ChamberStatus';

type ChamberStatusFormProps = RestFormProps<ChamberStatus> & WithTheme;

class ChamberStatusForm extends Component<ChamberStatusFormProps> {

  componentDidMount() {
    this.props.loadData();
  }

  sensorTemp(sensorId: string, sensors: SensorTemperatures) {
    if (sensors[sensorId]) {
      return sensors[sensorId].temp_c.toFixed(1) + "°C";
    }
    return 'Unknown'
  }

  render() {
    const { data, loadData, theme } = this.props;

    return (
      <Fragment>
        <List>
          <ListItem>
            <HighlightAvatar color={chamberStatusHighlight(data, theme)}>
              <KitchenIcon />
            </HighlightAvatar>
            <ListItemText primary="Chamber Status" secondary={chamberStatus(data)} />
          </ListItem>
          <ListItem>
            <Avatar>
              <TagFacesIcon />
            </Avatar>
            <ListItemText primary="Target Temperature" secondary={data.target_temp + "°C"} />
          </ListItem>
          <ListItem>
            <Avatar>
              <KitchenIcon />
            </Avatar>
            <ListItemText primary="Chamber Temperature" secondary={this.sensorTemp(data.chamber_sensor_address, data.sensors)} />
          </ListItem>
          <ListItem>
            <Avatar>
              <NaturePeopleIcon />
            </Avatar>
            <ListItemText primary="Ambient Temperature" secondary={this.sensorTemp(data.ambient_sensor_address, data.sensors)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <HighlightAvatar color={chamberEnabledHighlight(data.enable_cooler, theme)}>
              <AcUnitIcon />
            </HighlightAvatar>
            <ListItemText primary="Cooler Enabled?" secondary={data.enable_cooler ? "Enabled" : "Disabled"} />
          </ListItem>
          <ListItem>
            <HighlightAvatar color={chamberEnabledHighlight(data.enable_heater, theme)}>
              <WbSunnyIcon />
            </HighlightAvatar>
            <ListItemText primary="Heater Enabled?" secondary={data.enable_heater ? "Enabled" : "Disabled"} />
          </ListItem>
        </List>
        <FormActions>
          <FormButton variant="contained" color="secondary" onClick={loadData}>
            Refresh
          </FormButton>
        </FormActions>
      </Fragment>
    )
  }
}

export default withTheme(ChamberStatusForm);
