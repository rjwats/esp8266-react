import React, { FC } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Checkbox } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { ENDPOINT_ROOT } from '../api';
import { FormActions, FormButton, SectionContent, BlockFormControlLabel, FormLoader } from '../components';

import { LightState } from './types';
import { useRest } from '../hooks';
import { updateValue } from '../utils';

export const LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "lightState";

const LightStateRestController: FC = () => {
  const { loadData, saveData, saving, setData, data, errorMessage } = useRest<LightState>({ endpoint: LIGHT_SETTINGS_ENDPOINT });
  const updateFormValue = updateValue(setData);

  const content = () => {
    if (!data) {
      return (<FormLoader loadData={loadData} errorMessage={errorMessage} />);
    }

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
              name="led_on"
              disabled={saving}
              checked={data.led_on}
              onChange={updateFormValue}
              color="primary"
            />
          }
          label="LED State?"
        />
        <FormActions>
          <FormButton startIcon={<SaveIcon />} disabled={saving} variant="contained" color="primary" type="submit">
            Save
          </FormButton>
        </FormActions>
      </ValidatorForm>
    );
  }

  return (
    <SectionContent title='REST Controller' titleGutter>
      {content()}
    </SectionContent>
  );
}

export default LightStateRestController;
