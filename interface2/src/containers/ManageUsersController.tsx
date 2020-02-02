import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import ManageUsersForm from '../forms/ManageUsersForm';
import { SecuritySettings } from './SecuritySettingsController';
import { SECURITY_SETTINGS_ENDPOINT } from '../constants/Endpoints';

type ManageUsersControllerProps = RestControllerProps<SecuritySettings>;

class ManageUsersController extends Component<ManageUsersControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Manage Users" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <ManageUsersForm {...formProps} />}
        />
      </SectionContent>
    )
  }

}

export default restController(SECURITY_SETTINGS_ENDPOINT, ManageUsersController);
