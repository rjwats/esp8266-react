import React, { Component } from 'react';
import { ENDPOINT_ROOT, WEB_SOCKET_ROOT } from '../api';

import { SectionContent, webSocketController, WebSocketControllerProps, WebSocketFormLoader } from '../components';
import AudioLightSettingsForm from './AudioLightSettingsForm';
import { AudioLightMode } from './types';

export const AUDIO_LIGHT_SAVE_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "saveModeSettings";
export const AUDIO_LIGHT_LOAD_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "loadModeSettings";
export const AUDIO_LIGHT_SETTINGS_ENDPOINT = WEB_SOCKET_ROOT + "audioLightSettings";

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