import React, { Component } from 'react';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';

import { Typography, Box } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent } from '../components';

import { LightMqttSettings } from './types';

export const LIGHT_BROKER_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "brokerSettings";

type LightMqttSettingsControllerProps = RestControllerProps<LightMqttSettings>;

class LightMqttSettingsController extends Component<LightMqttSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='MQTT Controller' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <LightMqttSettingsControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default restController(LIGHT_BROKER_SETTINGS_ENDPOINT, LightMqttSettingsController);

type LightMqttSettingsControllerFormProps = RestFormProps<LightMqttSettings>;

function LightMqttSettingsControllerForm(props: LightMqttSettingsControllerFormProps) {
  const { data, saveData, handleValueChange } = props;
  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The LED is controllable via MQTT with the demo project designed to work with Home Assistant's auto discovery feature.
        </Typography>
      </Box>
      <TextValidator
        validators={['required']}
        errorMessages={['Unique ID is required']}
        name="unique_id"
        label="Unique ID"
        fullWidth
        variant="outlined"
        value={data.unique_id}
        onChange={handleValueChange('unique_id')}
        margin="normal"
      />
      <TextValidator
        validators={['required']}
        errorMessages={['Name is required']}
        name="name"
        label="Name"
        fullWidth
        variant="outlined"
        value={data.name}
        onChange={handleValueChange('name')}
        margin="normal"
      />
      <TextValidator
        validators={['required']}
        errorMessages={['MQTT Path is required']}
        name="mqtt_path"
        label="MQTT Path"
        fullWidth
        variant="outlined"
        value={data.mqtt_path}
        onChange={handleValueChange('mqtt_path')}
        margin="normal"
      />
      <FormActions>
        <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
          Save
        </FormButton>
      </FormActions>
    </ValidatorForm>
  );
}
