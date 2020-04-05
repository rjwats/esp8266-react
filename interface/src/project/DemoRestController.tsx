import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Checkbox } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent, BlockFormControlLabel } from '../components';

import { DemoSettings } from './types';

export const DEMO_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "demoSettings";

type DemoRestControllerProps = RestControllerProps<DemoSettings>;

class DemoRestController extends Component<DemoRestControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='REST Controller' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <DemoRestControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default restController(DEMO_SETTINGS_ENDPOINT, DemoRestController);

type DemoRestControllerFormProps = RestFormProps<DemoSettings>;

function DemoRestControllerForm(props: DemoRestControllerFormProps) {
  const { data, saveData, loadData, handleValueChange } = props;
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
        <FormButton variant="contained" color="secondary" onClick={loadData}>
          Reset
        </FormButton>
      </FormActions>
    </ValidatorForm>
  );
}
