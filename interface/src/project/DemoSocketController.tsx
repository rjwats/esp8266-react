import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Switch } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { SocketControllerProps, SocketFormLoader, SocketFormProps, socketController } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

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
  const { data, saveData, setData } = props;

  const changeLedOn = (event: React.ChangeEvent<HTMLInputElement>) => {
    setData({ led_on: event.target.checked }, saveData);
  }

  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The switch below controls the LED via the WebSocket, its state will automatically update whenever the LED state changes.
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
