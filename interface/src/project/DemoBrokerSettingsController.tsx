import React, { Component } from 'react';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';

import { Typography, Box } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent } from '../components';

import { DemoBrokerSettings } from './types';

export const DEMO_BROKER_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "lightSettings";

type DemoBrokerSettingsControllerProps = RestControllerProps<DemoBrokerSettings>;

class DemoBrokerSettingsController extends Component<DemoBrokerSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='REST Controller' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <DemoBrokerSettingsControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default restController(DEMO_BROKER_SETTINGS_ENDPOINT, DemoBrokerSettingsController);

type DemoBrokerSettingsControllerFormProps = RestFormProps<DemoBrokerSettings>;

function DemoBrokerSettingsControllerForm(props: DemoBrokerSettingsControllerFormProps) {
  const { data, saveData, loadData, handleValueChange } = props;
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
        <FormButton variant="contained" color="secondary" onClick={loadData}>
          Reset
        </FormButton>
      </FormActions>
    </ValidatorForm>
  );
}
