import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Checkbox } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent, BlockFormControlLabel } from '../components';

import { LightState } from './types';

export const LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "lightState";

type LightStateRestControllerProps = RestControllerProps<LightState>;

class LightStateRestController extends Component<LightStateRestControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='REST Controller' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <LightStateRestControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default restController(LIGHT_SETTINGS_ENDPOINT, LightStateRestController);

type LightStateRestControllerFormProps = RestFormProps<LightState>;

function LightStateRestControllerForm(props: LightStateRestControllerFormProps) {
  const { data, saveData, handleValueChange } = props;
  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The form below controls the LED via the RESTful service exposed by the ESP device.
        </Typography>
      </Box>
      <BlockFormControlLabel
        control={
          <Checkbox
            checked={data.led_on}
            onChange={handleValueChange('led_on')}
            color="primary"
          />
        }
        label="LED State?"
      />
      <FormActions>
        <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
          Save
        </FormButton>
      </FormActions>
    </ValidatorForm>
  );
}
