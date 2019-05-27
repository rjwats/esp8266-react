import React, { Component } from 'react';

import { SECURITY_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import SecuritySettingsForm from '../forms/SecuritySettingsForm';
import SectionContent from '../components/SectionContent';

class SecuritySettings extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="Security Settings">
        <SecuritySettingsForm
          securitySettings={data}
          securitySettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
        />
      </SectionContent>
    )
  }

}

export default restComponent(SECURITY_SETTINGS_ENDPOINT, SecuritySettings);
