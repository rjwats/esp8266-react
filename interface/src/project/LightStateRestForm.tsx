import { FC } from 'react';

import { Button, Checkbox } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import { SectionContent, FormLoader, BlockFormControlLabel, ButtonRow, MessageBox } from '../components';
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
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <MessageBox
          level="info"
          message="The form below controls the LED via the RESTful service exposed by the ESP device."
          my={2}
        />
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
        <ButtonRow mt={1}>
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
