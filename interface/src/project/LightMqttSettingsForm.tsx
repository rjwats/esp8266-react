import { FC, useState } from "react";
import { ValidateFieldsError } from "async-validator";

import { Button } from "@mui/material";
import SaveIcon from '@mui/icons-material/Save';

import { ButtonRow, FormLoader, MessageBox, SectionContent, ValidatedTextField } from "../components";
import { validate } from "../validators";
import { useRest, updateValue } from "../utils";

import * as DemoApi from './api';
import { LightMqttSettings } from "./types";
import { LIGHT_MQTT_SETTINGS_VALIDATOR } from "./validators";

const LightMqttSettingsForm: FC = () => {
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();
  const {
    loadData, saveData, saving, setData, data, errorMessage
  } = useRest<LightMqttSettings>({ read: DemoApi.readBrokerSettings, update: DemoApi.updateBrokerSettings });

  const updateFormValue = updateValue(setData);

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    const validateAndSubmit = async () => {
      try {
        setFieldErrors(undefined);
        await validate(LIGHT_MQTT_SETTINGS_VALIDATOR, data);
        saveData();
      } catch (errors: any) {
        setFieldErrors(errors);
      }
    };

    return (
      <>
        <MessageBox
          level="info"
          message="The LED is controllable via MQTT with the demo project designed to work with Home Assistant's auto discovery feature."
          my={2}
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="unique_id"
          label="Unique ID"
          fullWidth
          variant="outlined"
          value={data.unique_id}
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="name"
          label="Name"
          fullWidth
          variant="outlined"
          value={data.name}
          onChange={updateFormValue}
          margin="normal"
        />
        <ValidatedTextField
          fieldErrors={fieldErrors}
          name="mqtt_path"
          label="MQTT Path"
          fullWidth
          variant="outlined"
          value={data.mqtt_path}
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
    <SectionContent title='MQTT Settings' titleGutter>
      {content()}
    </SectionContent>
  );
};

export default LightMqttSettingsForm;
