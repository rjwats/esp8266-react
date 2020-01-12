import React, { Component } from 'react';

import { SECURITY_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import SecuritySettingsForm from '../forms/SecuritySettingsForm';
import SectionContent from '../components/SectionContent';
import RestFormLoader from '../components/RestFormLoader';
import { RestControllerProps, restController } from '../components/RestController';

// TODO - replace user authenticated context user with "Me", this one is the User type.
export interface TempUser {
  username: string;
  password: string;
  admin: boolean;
}

export interface SecuritySettingsData {
  users: TempUser[];
  jwt_secret: string;
}

class SecuritySettings extends Component<RestControllerProps<SecuritySettingsData>> {

  componentDidMount() {
    this.props.loadData();
  }
  
  render() {
    return (
      <SectionContent title="Security Settings">
        <RestFormLoader
          {...this.props}
          render={formProps => <SecuritySettingsForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(SECURITY_SETTINGS_ENDPOINT, SecuritySettings);
