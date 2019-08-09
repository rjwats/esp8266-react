import React, { Component } from 'react';

import { SECURITY_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';
import SectionContent from '../components/SectionContent';
import ManageUsersForm from '../forms/ManageUsersForm';

class ManageUsers extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { fetched, errorMessage, data, saveData, loadData, setData, handleValueChange } = this.props;
    return (
      <SectionContent title="Manage Users" titleGutter>
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={() =>
            <ManageUsersForm
              userData={data}
              onSubmit={saveData}
              onReset={loadData}
              setData={setData}
              handleValueChange={handleValueChange}
            />
          }
        />
      </SectionContent>
    )
  }

}

export default restComponent(SECURITY_SETTINGS_ENDPOINT, ManageUsers);
