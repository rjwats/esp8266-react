import React, { Component } from 'react';

import { SECURITY_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import SectionContent from '../components/SectionContent';
import ManageUsersForm from '../forms/ManageUsersForm';
import RestFormLoader from '../components/RestFormLoader';
import { restController, RestControllerProps } from '../components/RestController';
import { SecuritySettings } from './SecuritySettingsController';

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
