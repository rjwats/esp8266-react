import React, { FC, useState } from 'react';
import { ValidateFieldsError } from 'async-validator';

import { Button, Checkbox } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import * as SystemApi from "../../api/system";
import { OTASettings } from '../../types';
import { BlockFormControlLabel, ButtonRow, FormLoader, SectionContent, ValidatedPasswordField, ValidatedTextField } from '../../components';
import { validate, OTA_SETTINGS_VALIDATOR } from '../../validators';
import { numberValue, updateValue, useRest } from '../../utils';

const OTASettingsForm: FC = () => {
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();
  const {
    loadData, saving, data, setData, saveData, errorMessage
  } = useRest<OTASettings>({ read: SystemApi.readOTASettings, update: SystemApi.updateOTASettings });

  const updateFormValue = updateValue(setData);

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    const validateAndSubmit = async () => {
      try {
        setFieldErrors(undefined);
        await validate(OTA_SETTINGS_VALIDATOR, data);
        saveData();
      } catch (errors: any) {
        setFieldErrors(errors);
      }
    };

    return (
      <>
        <BlockFormControlLabel
          control={
            <Checkbox
              name="enabled"
              checked={data.enabled}
              onChange={updateFormValue}
            />
          }
          label="Enable OTA Updates?"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="port"
          label="Port"
          fullWidth
          variant="outlined"
          value={numberValue(data.port)}
          type="number"
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedPasswordField
          fieldErrors={fieldErrors}
          name="password"
          label="Password"
          fullWidth
          variant="outlined"
          value={data.password}
          onChange={updateFormValue}
          margin="normal"
        />
        <ButtonRow mt={1}>
          <Button startIcon={<SaveIcon />} disabled={saving} variant="contained" color="primary" type="submit" onClick={validateAndSubmit}>
            Save
          </Button>
        </ButtonRow>
      </>
    );
  };

  return (
    <SectionContent title='OTA Settings' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default OTASettingsForm;
