import React, { Component } from 'react';
import { LIGHT_STRIP_ENDPOINT, WS_URI }  from  '../constants/Endpoints';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import LightStripSettingsForm from '../forms/LightStripSettingsForm';

import Sockette from 'sockette';

class LightStripSettings extends Component {

  componentDidMount() {
    this.props.loadData();
    this.handleChangeMode = this.handleChangeMode.bind(this);
    const ws = new Sockette(WS_URI, {
      onopen: e => console.log('Connected!', e),
      onmessage: e => console.log('Received:', e),
      onreconnect: e => console.log('Reconnecting...', e),
      onmaximum: e => console.log('Stop Attempting!', e),
      onclose: e => console.log('Closed!', e),
      onerror: e => console.log('Error:', e)
    });
    this.setState({ws});
  }

  componentWillUnmount() {
    this.state.ws.close();
  }

  handleChangeMode(event) {
    this.props.setDataAndSave({mode_code: event.target.value}, true, false);
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="Light Strip Settings">
        <LightStripSettingsForm
          lightStripSettings={data}
          lightStripSettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
          handleColorChange={this.props.handleColorChange}
          handleChange={this.props.handleChange}
          handleChangeMode={this.handleChangeMode}
        />
      </SectionContent>
    );
  }

}

export default restComponent(LIGHT_STRIP_ENDPOINT, LightStripSettings);
