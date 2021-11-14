import { FC, useState } from "react";
import { ValidateFieldsError } from "async-validator";

import { Box, Button, Typography } from "@mui/material";
import SaveIcon from '@mui/icons-material/Save';

import { ButtonRow, FormLoader, SectionContent, ValidatedTextField } from "../components";
import { useRest, updateValue } from "../utils";
import { validate } from "../validators";

import { readBrokerSettings, updateBrokerSettings } from "./api";
import { LIGHT_MQTT_SETTINGS_VALIDATOR } from "./validators";
import { LightMqttSettings } from "./types";

const LightMqttSettingsForm: FC = () => {
  const {
    loadData, saveData, saving, setData, data, errorMessage
  } = useRest<LightMqttSettings>({ read: readBrokerSettings, update: updateBrokerSettings });

  const updateFormValue = updateValue(setData);

  // TODO - extend the above hook to validate the input on submit and only save to the backend if valid.
  // NB: Saving must be asserted while validation takes place
  const [fieldErrors, setFieldErrors] = useState<ValidateFieldsError>();

  const validateAndSubmit = async () => {
    if (data) {
      try {
        await validate(LIGHT_MQTT_SETTINGS_VALIDATOR, data);
        saveData();
      } catch (errors: any) {
        setFieldErrors(errors);
      }
    }
  };

  const content = () => {
    if (!data) {
      return (<FormLoader loadData={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
          <Typography variant="body1">
            The LED is controllable via MQTT with the demo project designed to work with Home Assistant's auto discovery feature.
          </Typography>
        </Box>
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
        <ButtonRow>
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
