import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { SECURITY_SETTINGS_ENDPOINT } from '../constants/Endpoints';

import SecuritySettingsForm from './SecuritySettingsForm';

export interface User {
  username: string;
  password: string;
  admin: boolean;
}

export interface SecuritySettings {
  users: User[];
  jwt_secret: string;
}

type SecuritySettingsControllerProps = RestControllerProps<SecuritySettings>;

class SecuritySettingsController extends Component<SecuritySettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Security Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <SecuritySettingsForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(SECURITY_SETTINGS_ENDPOINT, SecuritySettingsController);
