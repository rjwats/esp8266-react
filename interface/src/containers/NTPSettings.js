import React, { Component } from 'react';

import { NTP_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import SectionContent from '../components/SectionContent';
import SnackbarNotification from '../components/SnackbarNotification';
import NTPSettingsForm from '../forms/NTPSettingsForm';
import { simpleGet }  from  '../helpers/SimpleGet';
import { simplePost } from '../helpers/SimplePost';

class NTPSettings extends Component {

  constructor(props) {
    super(props);

    this.state = {
             ntpSettings:{},
             ntpSettingsFetched: false,
             errorMessage:null
           };

    this.setState = this.setState.bind(this);
    this.loadNTPSettings = this.loadNTPSettings.bind(this);
    this.saveNTPSettings = this.saveNTPSettings.bind(this);
  }

  componentDidMount() {
      this.loadNTPSettings();
  }

  loadNTPSettings() {
    simpleGet(
      NTP_SETTINGS_ENDPOINT,
      this.setState,
      this.raiseNotification,
      "ntpSettings",
      "ntpSettingsFetched"
    );
  }

  saveNTPSettings(e) {
    simplePost(
      NTP_SETTINGS_ENDPOINT,
      this.state,
      this.setState,
      this.raiseNotification,
      "ntpSettings",
      "ntpSettingsFetched"
    );
  }

  ntpSettingValueChange = name => event => {
    const { ntpSettings } = this.state;
    ntpSettings[name] = event.target.value;
    this.setState({ntpSettings});
  };

  render() {
    const { ntpSettingsFetched, ntpSettings, errorMessage } = this.state;
    return (
      <SectionContent title="NTP Settings">
        <SnackbarNotification notificationRef={(raiseNotification)=>this.raiseNotification = raiseNotification} />
      	<NTPSettingsForm ntpSettingsFetched={ntpSettingsFetched} ntpSettings={ntpSettings} errorMessage={errorMessage}
          onSubmit={this.saveNTPSettings} onReset={this.loadNTPSettings} handleValueChange={this.ntpSettingValueChange} />
      </SectionContent>
    )
  }

}

export default NTPSettings;
