import React, { Component } from 'react';

import { AP_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import {withNotifier} from '../components/SnackbarNotification';
import SectionContent from '../components/SectionContent';
import APSettingsForm from '../forms/APSettingsForm';
import { simpleGet } from  '../helpers/SimpleGet';
import { simplePost } from '../helpers/SimplePost';

class APSettings extends Component {

  constructor(props) {
    super(props);

    this.state = {
             apSettings:null,
             apSettingsFetched: false,
             errorMessage:null
           };

    this.setState = this.setState.bind(this);
    this.loadAPSettings = this.loadAPSettings.bind(this);
    this.saveAPSettings = this.saveAPSettings.bind(this);
  }

  componentDidMount() {
    this.loadAPSettings();
  }

  loadAPSettings() {
    simpleGet(
      AP_SETTINGS_ENDPOINT,
      this.setState,
      this.props.raiseNotification,
      "apSettings",
      "apSettingsFetched"
    );
  }

  saveAPSettings(e) {
    simplePost(
      AP_SETTINGS_ENDPOINT,
      this.state,
      this.setState,
      this.props.raiseNotification,
      "apSettings",
      "apSettingsFetched"
    );
  }

  wifiSettingValueChange = name => event => {
    const { apSettings } = this.state;
    apSettings[name] = event.target.value;
    this.setState({apSettings});
  };

  render() {
    const { apSettingsFetched, apSettings, errorMessage } = this.state;
    return (
      <SectionContent title="AP Settings">
      	<APSettingsForm  apSettingsFetched={apSettingsFetched} apSettings={apSettings} errorMessage={errorMessage}
          onSubmit={this.saveAPSettings} onReset={this.loadAPSettings} handleValueChange={this.wifiSettingValueChange} />
      </SectionContent>
    )
  }

}

export default withNotifier(APSettings);
