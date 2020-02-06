import React, { Component } from 'react';

import { AP_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';

import APSettingsForm from './APSettingsForm';

export interface APSettings {
  provision_mode: number;
  ssid: string;
  password: string;
}

type APSettingsControllerProps = RestControllerProps<APSettings>;

class APSettingsController extends Component<APSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Access Point Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <APSettingsForm {...formProps} />}
        />
      </SectionContent>
    )
  }

}

export default restController(AP_SETTINGS_ENDPOINT, APSettingsController);
