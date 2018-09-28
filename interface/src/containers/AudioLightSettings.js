import React, { Component } from 'react';
import { AUDIO_LIGHT_WEBSOCKET_URI } from '../constants/Endpoints';
import { wsComponent } from '../components/WSComponent';
import SectionContent from '../components/SectionContent';
import AudioLightSettingsForm from '../forms/AudioLightSettingsForm';

class LightStripSettings extends Component {

  constructor(props) {
    super(props);
    this.handleChangeMode = this.handleChangeMode.bind(this);
  }

  handleChangeMode(event) {
    this.props.setDataAndReset(
      {
        mode_id: event.target.value
      }
    );
  }

  render() {
    const {
      data,
      fetched,
      errorMessage,
      saveData,
      handleValueChange,
      handleColorChange,
      handleChange
    } = this.props;

    return (
      <SectionContent title="Settings">
        <AudioLightSettingsForm
          audioLightSettings={data}
          audioLightSettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={saveData}
          handleValueChange={handleValueChange}
          handleColorChange={handleColorChange}
          handleChange={handleChange}
          handleChangeMode={this.handleChangeMode}
        />
      </SectionContent>
    );
  }

}

export default wsComponent(AUDIO_LIGHT_WEBSOCKET_URI, LightStripSettings);
