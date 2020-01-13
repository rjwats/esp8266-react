import React, { Component } from 'react';

import RestFormLoader from '../components/RestFormLoader';
import SectionContent from '../components/SectionContent';
import { restController, RestControllerProps} from '../components/RestController';
import OTASettingsForm from '../forms/OTASettingsForm';
import { OTA_SETTINGS_ENDPOINT } from '../constants/Endpoints';

export interface OTASettingsData {
  enabled: boolean;
  port: number;
  password: string;
}

class OTASettings extends Component<RestControllerProps<OTASettingsData>> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="OTA Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <OTASettingsForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(OTA_SETTINGS_ENDPOINT, OTASettings);
