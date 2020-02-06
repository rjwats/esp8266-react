import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { OTA_SETTINGS_ENDPOINT } from '../constants/Endpoints';

import OTASettingsForm from './OTASettingsForm';

export interface OTASettings {
  enabled: boolean;
  port: number;
  password: string;
}

type OTASettingsControllerProps = RestControllerProps<OTASettings>;

class OTASettingsController extends Component<OTASettingsControllerProps> {

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

export default restController(OTA_SETTINGS_ENDPOINT, OTASettingsController);
