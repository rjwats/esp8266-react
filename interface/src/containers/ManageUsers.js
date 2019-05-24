import React, { Component } from 'react';

import { USERS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import ManageUsersForm from '../forms/ManageUsersForm';

class ManageUsers extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <ManageUsersForm
        userData={data}
        userDataFetched={fetched}
        errorMessage={errorMessage}
        onSubmit={this.props.saveData}
        onReset={this.props.loadData}
        setData={this.props.setData}
        handleValueChange={this.props.handleValueChange}
      />
    )
  }

}

export default restComponent(USERS_ENDPOINT, ManageUsers);
