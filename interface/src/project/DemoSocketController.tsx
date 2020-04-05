import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Checkbox } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { WEB_SOCKET_ROOT } from '../api';
import { SocketControllerProps, SocketFormLoader, SocketFormProps, SectionContent, BlockFormControlLabel, socketController, FormActions, FormButton } from '../components';

import { DemoSettings } from './types';

export const DEMO_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "/demoSettings";

type DemoSocketControllerProps = SocketControllerProps<DemoSettings>;

class DemoSocketController extends Component<DemoSocketControllerProps> {

  render() {
    return (
      <SectionContent title='WebSocket Controller' titleGutter>
        <SocketFormLoader
          {...this.props}
          render={props => (
            <DemoSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default socketController(DEMO_SETTINGS_WEBSOCKET_URL, 100, DemoSocketController);

type DemoSocketControllerFormProps = SocketFormProps<DemoSettings>;

function DemoSocketControllerForm(props: DemoSocketControllerFormProps) {
  const { data, saveData, handleValueChange } = props;
  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The form below controls the LED via the WebSocket exposed by the ESP device.
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
