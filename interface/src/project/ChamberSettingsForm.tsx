import React, { Component } from 'react';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import { MenuItem, Checkbox } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { ChamberSettings, SensorTemperatures, ChamberStatus } from './types';
import { RestFormProps, FormActions, FormButton, BlockFormControlLabel } from '../components';

interface ChamberSettingsFormProps extends RestFormProps<ChamberSettings> {
  chamberStatus: ChamberStatus;
}

class ChamberSettingsForm extends Component<ChamberSettingsFormProps> {

  sensorMenuItem(sensorId: string, sensors: SensorTemperatures) {
    return <MenuItem key={sensorId} value={sensorId}>{sensorId} - {sensors[sensorId].temp_c.toFixed(1)} &#x2103;</MenuItem>
  }

  createSensorMenuItems() {
    const { chamberStatus } = this.props;
    return (chamberStatus.sensors ? Object.keys(chamberStatus.sensors) : []).map(sensorId => this.sensorMenuItem(sensorId, chamberStatus.sensors));
  }

  sensorValue(sensorId: string) {
    const { chamberStatus } = this.props;
    return chamberStatus.sensors ? chamberStatus.sensors[sensorId] ? sensorId : "" : "";
  }

  render() {
    const { data, handleValueChange, handleCheckboxChange, saveData, loadData } = this.props;
    return (
      <ValidatorForm onSubmit={saveData}>
        <SelectValidator
          name="chamber_sensor_address"
          labelId="chamber_sensor_address"
          label="Chamber Sensor"
          fullWidth
          native
          value={this.sensorValue(data.chamber_sensor_address)}
          onChange={handleValueChange('chamber_sensor_address')}
          margin="normal"
        >
          <MenuItem value="">
            <em>Select chamber sensor...</em>
          </MenuItem>
          {this.createSensorMenuItems()}
        </SelectValidator>
        <SelectValidator
          name="ambient_sensor_address"
          labelId="ambient_sensor_address"
          label="Ambient Sensor"
          fullWidth
          native
          value={this.sensorValue(data.ambient_sensor_address)}
          onChange={handleValueChange('ambient_sensor_address')}
          margin="normal"
        >
          <MenuItem value="">
            <em>Select ambient sensor...</em>
          </MenuItem>
          {this.createSensorMenuItems()}
        </SelectValidator>
        <TextValidator
          validators={['required', 'isFloat', 'minNumber:0', 'maxNumber:25']}
          errorMessages={['Target temperature is required', "Must be a number", "Must be greater than 0 ", "Max value is 25"]}
          name="target_temp"
          label="Target temperature"
          fullWidth
          variant="outlined"
          value={data.target_temp}
          type="number"
          onChange={handleValueChange('target_temp')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isFloat', 'minNumber:0', 'maxNumber:5']}
          errorMessages={['Hysteresis high is required', "Must be a number", "Must be greater than 0 ", "Max value is 5"]}
          name="hysteresis_high"
          label="Hysteresis high"
          fullWidth
          variant="outlined"
          value={data.hysteresis_high}
          type="number"
          onChange={handleValueChange('hysteresis_high')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isFloat', 'minNumber:0', 'maxNumber:5']}
          errorMessages={['Hysteresis low is required', "Must be a number", "Must be greater than 0 ", "Max value is 5"]}
          name="hysteresis_low"
          label="Hysteresis low"
          fullWidth
          variant="outlined"
          value={data.hysteresis_low}
          type="number"
          onChange={handleValueChange('hysteresis_low')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isFloat', 'minNumber:0', 'maxNumber:1']}
          errorMessages={['Hysteresis low is required', "Must be a number", "Must be a positive value", "Must be no greater than 1"]}
          name="hysteresis_factor"
          label="Hysteresis factor"
          fullWidth
          variant="outlined"
          value={data.hysteresis_factor}
          type="number"
          onChange={handleValueChange('hysteresis_factor')}
          margin="normal"
        />
        <BlockFormControlLabel
          control={
            <Checkbox
              checked={data.enable_heater}
              onChange={handleCheckboxChange('enable_heater')}
            />
          }
          label="Enable Heater?"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:3600000']}
          errorMessages={['Min heater on duration is required', "Must be a number", "Must be greater than 0 ", "Max value is 3600000"]}
          name="min_heater_on_duration"
          label="Min heater on duration"
          fullWidth
          variant="outlined"
          value={data.min_heater_on_duration}
          type="number"
          onChange={handleValueChange('min_heater_on_duration')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:3600000']}
          errorMessages={['Min heater off duration is required', "Must be a number", "Must be greater than 0 ", "Max value is 3600000"]}
          name="min_heater_off_duration"
          label="Min heater off duration"
          fullWidth
          variant="outlined"
          value={data.min_heater_off_duration}
          type="number"
          onChange={handleValueChange('min_heater_off_duration')}
          margin="normal"
        />
        <BlockFormControlLabel
          control={
            <Checkbox
              checked={data.enable_cooler}
              onChange={handleCheckboxChange('enable_cooler')}
            />
          }
          label="Enable Cooler?"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:3600000']}
          errorMessages={['Min cooler on duration is required', "Must be a number", "Must be greater than 0 ", "Max value is 3600000"]}
          name="min_cooler_on_duration"
          label="Min cooler on duration"
          fullWidth
          variant="outlined"
          value={data.min_cooler_on_duration}
          type="number"
          onChange={handleValueChange('min_cooler_on_duration')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:3600000']}
          errorMessages={['Min cooler off duration is required', "Must be a number", "Must be greater than 0 ", "Max value is 3600000"]}
          name="min_cooler_off_duration"
          label="Min cooler off duration"
          fullWidth
          variant="outlined"
          value={data.min_cooler_off_duration}
          type="number"
          onChange={handleValueChange('min_cooler_off_duration')}
          margin="normal"
        />
        <FormActions>
          <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
            Save
          </FormButton>
          <FormButton variant="contained" color="secondary" onClick={loadData}>
            Reset
          </FormButton>
        </FormActions>
      </ValidatorForm>
    );
  }
}

export default ChamberSettingsForm;
