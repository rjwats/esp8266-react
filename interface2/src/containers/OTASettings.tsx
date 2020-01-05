import React, { Component } from 'react';

import { OTA_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import SectionContent from '../components/SectionContent';
import OTASettingsForm from '../forms/OTASettingsForm';
import { restController, RestControllerProps} from '../components/RestController';
import RestFormLoader from '../components/RestFormLoader';

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
      <SectionContent title="OTA Settings">
        <RestFormLoader
          {...this.props}
          render={formProps => <OTASettingsForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(OTA_SETTINGS_ENDPOINT, OTASettings);
