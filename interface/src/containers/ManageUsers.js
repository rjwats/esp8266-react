import React, { Component } from 'react';

import { SECURITY_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import ManageUsersForm from '../forms/ManageUsersForm';
import SectionContent from '../components/SectionContent';

class ManageUsers extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="Manage Users">
        <ManageUsersForm
          userData={data}
          userDataFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          setData={this.props.setData}
          handleValueChange={this.props.handleValueChange}
        />
      </SectionContent>
    )
  }

}

export default restComponent(SECURITY_SETTINGS_ENDPOINT, ManageUsers);
