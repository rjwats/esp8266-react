import React, { Component } from 'react';
import { WEB_SOCKET_ROOT } from '../api';

import { SectionContent, webSocketController, WebSocketControllerProps, WebSocketFormLoader } from '../components';
import AudioLightSettingsForm from './AudioLightSettingsForm';
import { AudioLightSettings } from './types';

export const AUDIO_LIGHT_SETTINGS_ENDPOINT = WEB_SOCKET_ROOT + "audioLightSettings";

type AudioLightSettingsControllerProps = WebSocketControllerProps<AudioLightSettings>;

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