import React, { Component, Fragment } from 'react';

import { Divider, List, Avatar, ListItemAvatar, ListItemText, ListItem, withTheme, WithTheme } from '@material-ui/core';

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
            <ListItemAvatar>
              <HighlightAvatar color={chamberStatusHighlight(data, theme)}>
                <KitchenIcon />
              </HighlightAvatar>
            </ListItemAvatar>
            <ListItemText primary="Chamber Status" secondary={chamberStatus(data)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <Avatar>
                <TagFacesIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Target Temperature" secondary={data.target_temp + "°C"} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <Avatar>
                <KitchenIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Chamber Temperature" secondary={this.sensorTemp(data.chamber_sensor_address, data.sensors)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <Avatar>
                <NaturePeopleIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Ambient Temperature" secondary={this.sensorTemp(data.ambient_sensor_address, data.sensors)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <HighlightAvatar color={chamberEnabledHighlight(data.enable_cooler, theme)}>
                <AcUnitIcon />
              </HighlightAvatar>
            </ListItemAvatar>
            <ListItemText primary="Cooler Enabled?" secondary={data.enable_cooler ? "Enabled" : "Disabled"} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <HighlightAvatar color={chamberEnabledHighlight(data.enable_heater, theme)}>
                <WbSunnyIcon />
              </HighlightAvatar>
            </ListItemAvatar>
            <ListItemText primary="Heater Enabled?" secondary={data.enable_heater ? "Enabled" : "Disabled"} />
          </ListItem>
          <Divider variant="inset" component="li" />
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
