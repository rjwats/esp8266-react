import React, { Component } from 'react';
import { LIGHT_STRIP_WEBSOCKET_URI }  from  '../constants/Endpoints';
import {wsComponent} from '../components/WSComponent';
import SectionContent from '../components/SectionContent';
import LightStripSettingsForm from '../forms/LightStripSettingsForm';

class LightStripSettings extends Component {

  constructor(props) {
    super(props);
    this.handleChangeMode = this.handleChangeMode.bind(this);
  }

  handleChangeMode(event) {
    this.props.setDataAndReset({mode_code: event.target.value});
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
      <SectionContent title="Light Strip Settings">
        <LightStripSettingsForm
          lightStripSettings={data}
          lightStripSettingsFetched={fetched}
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

export default wsComponent(LIGHT_STRIP_WEBSOCKET_URI, LightStripSettings);
