import { Container } from '@material-ui/core';
import React, { Component } from 'react';

import { SectionContent, webSocketController, WebSocketControllerProps, WebSocketFormLoader } from '../components';
import AudioLightSettingsForm from './AudioLightSettingsForm';
import PaletteSettingsLoader from './PaletteSettingsLoader';
import { AudioLightMode, AUDIO_LIGHT_SETTINGS_ENDPOINT } from './types';

type AudioLightSettingsControllerProps = WebSocketControllerProps<Partial<AudioLightMode>>;

class AudioLightSettingsController extends Component<AudioLightSettingsControllerProps> {

  render() {
    return (
      <Container maxWidth="md" disableGutters>
        <SectionContent title="Lighting Settings">
          <PaletteSettingsLoader>
            <WebSocketFormLoader
              {...this.props}
              render={formProps => <AudioLightSettingsForm {...formProps} />}
            />
          </PaletteSettingsLoader>
        </SectionContent>
      </Container>
    );
  }

}

export default webSocketController(AUDIO_LIGHT_SETTINGS_ENDPOINT, 100, AudioLightSettingsController);