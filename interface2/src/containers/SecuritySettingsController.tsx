import React, { Component } from 'react';

import { SECURITY_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import SecuritySettingsForm from '../forms/SecuritySettingsForm';
import SectionContent from '../components/SectionContent';
import RestFormLoader from '../components/RestFormLoader';
import { RestControllerProps, restController } from '../components/RestController';

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
