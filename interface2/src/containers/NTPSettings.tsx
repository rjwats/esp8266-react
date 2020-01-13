import React, { Component } from 'react';

import RestFormLoader from '../components/RestFormLoader';
import SectionContent from '../components/SectionContent';
import { restController, RestControllerProps} from '../components/RestController';
import NTPSettingsForm from '../forms/NTPSettingsForm';
import { NTP_SETTINGS_ENDPOINT } from '../constants/Endpoints';

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
      <SectionContent title="NTP Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <NTPSettingsForm {...formProps} />}
        />
      </SectionContent>
    )
  }

}

export default restController(NTP_SETTINGS_ENDPOINT, NTPSettings);
