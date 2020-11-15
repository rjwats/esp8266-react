import React, { Component } from 'react';
import { ENDPOINT_ROOT } from '../api';

import { restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import AudioLightSettingsForm from './AudioLightSettingsForm';
import { AudioLightSettings } from './types';

export const AUDIO_LIGHT_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "audioLightSettings";

type AudioLightSettingsControllerProps = RestControllerProps<AudioLightSettings>;

class AudioLightSettingsController extends Component<AudioLightSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Lighting Settings">
        <RestFormLoader
          {...this.props}
          render={formProps => <AudioLightSettingsForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(AUDIO_LIGHT_SETTINGS_ENDPOINT, AudioLightSettingsController);