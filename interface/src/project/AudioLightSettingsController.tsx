import React, { Component } from 'react';

import { SectionContent, webSocketController, WebSocketControllerProps, WebSocketFormLoader } from '../components';
import AudioLightSettingsForm from './AudioLightSettingsForm';
import { AudioLightMode, AUDIO_LIGHT_SETTINGS_ENDPOINT } from './types';

type AudioLightSettingsControllerProps = WebSocketControllerProps<Partial<AudioLightMode>>;

class AudioLightSettingsController extends Component<AudioLightSettingsControllerProps> {

  render() {
    return (
      <SectionContent title="Lighting Settings">
        <WebSocketFormLoader
          {...this.props}
          render={formProps => <AudioLightSettingsForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default webSocketController(AUDIO_LIGHT_SETTINGS_ENDPOINT, 100, AudioLightSettingsController);