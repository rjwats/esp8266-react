import { FC, useState } from 'react';
import { ValidateFieldsError } from 'async-validator';

import { Button, Checkbox} from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import * as SerialApi from "../../api/serial";
import { SerialSettings } from '../../types';
import { BlockFormControlLabel, ButtonRow, FormLoader, SectionContent, ValidatedTextField } from '../../components';
import { validate, SERIAL_SETTINGS_VALIDATOR } from '../../validators';
import { updateValue, useRest } from '../../utils';

const SerialSettingsForm: FC = () => {
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();
  const {
    loadData, saving, data, setData, saveData, errorMessage
  } = useRest<SerialSettings>({ read: SerialApi.readSerialSettings, update: SerialApi.updateSerialSettings });

  const updateFormValue = updateValue(setData);

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    const validateAndSubmit = async () => {
      try {
        setFieldErrors(undefined);
        await validate(SERIAL_SETTINGS_VALIDATOR, data);
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
          label="Enable Serial"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="rxpin"
          label="rx pin"
          fullWidth
          variant="outlined"
          value={data.rxpin}
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="txpin"
          label="tx pin"
          fullWidth
          variant="outlined"
          value={data.txpin}
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="baud"
          label="Baud rate"
          fullWidth
          variant="outlined"
          value={data.baud}
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="config"
          label="Config"
          fullWidth
          variant="outlined"
          value={data.config}
          onChange={updateFormValue}
          margin="normal"
        />
        <BlockFormControlLabel
          control={
            <Checkbox
            name="invert"
              checked={data.invert}
              onChange={updateFormValue}
            />
          }
          label="Inverted signal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="port"
          label="Port"
          fullWidth
          variant="outlined"
          value={data.port}
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
    <SectionContent title='Serial Settings' titleGutter>
      {content()}
    </SectionContent>
  );

};

export default SerialSettingsForm;
