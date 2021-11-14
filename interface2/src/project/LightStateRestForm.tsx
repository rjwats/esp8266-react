import { FC } from 'react';

import { Box, Button, Checkbox, Typography } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import { SectionContent, FormLoader, BlockFormControlLabel, ButtonRow } from '../components';
import { updateValue, useRest } from '../utils';

import * as DemoApi from './api';
import { LightState } from './types';

const LightStateRestForm: FC = () => {
  const {
    loadData, saveData, saving, setData, data, errorMessage
  } = useRest<LightState>({ read: DemoApi.readLightState, update: DemoApi.updateLightState });

  const updateFormValue = updateValue(setData);

  const content = () => {
    if (!data) {
      return (<FormLoader loadData={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
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
        <ButtonRow>
          <Button startIcon={<SaveIcon />} disabled={saving} variant="contained" color="primary" type="submit" onClick={saveData}>
            Save
          </Button>
        </ButtonRow>
      </>
    );
  };

  return (
    <SectionContent title='REST Example' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default LightStateRestForm;
