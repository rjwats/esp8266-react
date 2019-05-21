import React, { Component } from 'react';

import { USERS_ENDPOINT }  from  '../constants/Endpoints';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import ManageUsersForm from '../forms/ManageUsersForm';

class ManageUsers extends Component {

  componentDidMount() {
      this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="Manage Users">
      	<ManageUsersForm
          users={data}
          usersFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
          handleCheckboxChange={this.props.handleCheckboxChange}
        />
      </SectionContent>
    )
  }

}

export default restComponent(USERS_ENDPOINT, ManageUsers);
