import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Switch } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { SocketControllerProps, SocketFormLoader, SocketFormProps, socketController } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { LightSettings } from './types';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "/lightSettings";

type LightSettingsSocketControllerProps = SocketControllerProps<LightSettings>;

class LightSettingsSocketController extends Component<LightSettingsSocketControllerProps> {

  render() {
    return (
      <SectionContent title='WebSocket Controller' titleGutter>
        <SocketFormLoader
          {...this.props}
          render={props => (
            <LightSettingsSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default socketController(LIGHT_SETTINGS_WEBSOCKET_URL, 100, LightSettingsSocketController);

type LightSettingsSocketControllerFormProps = SocketFormProps<LightSettings>;

function LightSettingsSocketControllerForm(props: LightSettingsSocketControllerFormProps) {
  const { data, saveData, setData } = props;

  const changeLedOn = (event: React.ChangeEvent<HTMLInputElement>) => {
    setData({ led_on: event.target.checked }, saveData);
  }

  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The switch below controls the LED via the WebSocket. It will automatically update whenever the LED state changes.
        </Typography>
      </Box>
      <BlockFormControlLabel
        control={
          <Switch
            checked={data.led_on}
            onChange={changeLedOn}
            color="primary"
          />
        }
        label="LED State?"
      />
    </ValidatorForm>
  );
}
