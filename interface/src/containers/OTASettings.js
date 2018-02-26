import React, { Component } from 'react';

import { OTA_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import SectionContent from '../components/SectionContent';
import SnackbarNotification from '../components/SnackbarNotification';
import OTASettingsForm from '../forms/OTASettingsForm';
import { simpleGet }  from  '../helpers/SimpleGet';
import { simplePost } from '../helpers/SimplePost';

class OTASettings extends Component {

  constructor(props) {
    super(props);

    this.state = {
             otaSettings:null,
             otaSettingsFetched: false,
             errorMessage:null
           };

    this.setState = this.setState.bind(this);
    this.loadOTASettings = this.loadOTASettings.bind(this);
    this.saveOTASettings = this.saveOTASettings.bind(this);
  }

  componentDidMount() {
      this.loadOTASettings();
  }

  loadOTASettings() {
    simpleGet(
      OTA_SETTINGS_ENDPOINT,
      this.setState,
      this.raiseNotification,
      "otaSettings",
      "otaSettingsFetched"
    );
  }

  saveOTASettings(e) {
    simplePost(
      OTA_SETTINGS_ENDPOINT,
      this.state,
      this.setState,
      this.raiseNotification,
      "otaSettings",
      "otaSettingsFetched"
    );
  }

  otaSettingValueChange = name => event => {
    const { otaSettings } = this.state;
    otaSettings[name] = event.target.value;
    this.setState({otaSettings});
  };

  otaSettingCheckboxChange = name => event => {
    const { otaSettings } = this.state;
    otaSettings[name] = event.target.checked;
    this.setState({otaSettings});
  }

  render() {
    const { otaSettingsFetched, otaSettings, errorMessage } = this.state;
    return (
      <SectionContent title="OTA Settings">
        <SnackbarNotification notificationRef={(raiseNotification)=>this.raiseNotification = raiseNotification} />
      	<OTASettingsForm otaSettingsFetched={otaSettingsFetched} otaSettings={otaSettings} errorMessage={errorMessage}
          onSubmit={this.saveOTASettings} onReset={this.loadOTASettings} handleValueChange={this.otaSettingValueChange}
          handleCheckboxChange={this.otaSettingCheckboxChange} />
      </SectionContent>
    )
  }

}

export default OTASettings;
