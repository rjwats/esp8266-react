import React, { Component } from 'react';

import { NTP_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import SectionContent from '../components/SectionContent';
import NTPSettingsForm from '../forms/NTPSettingsForm';
import { RestControllerProps, restController } from '../components/RestController';
import RestFormLoader from '../components/RestFormLoader';

export interface NTPSettingsData {
  server: string;
  interval: number;
}

class NTPSettings extends Component<RestControllerProps<NTPSettingsData>> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="NTP Settings">
        <RestFormLoader
          {...this.props}
          render={formProps => <NTPSettingsForm {...formProps} />}
        />
      </SectionContent>
    )
  }

}

export default restController(NTP_SETTINGS_ENDPOINT, NTPSettings);
