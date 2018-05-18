import React, { Component } from 'react';

import { NTP_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import NTPSettingsForm from '../forms/NTPSettingsForm';

class NTPSettings extends Component {

  componentDidMount() {
      this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="NTP Settings">
      	<NTPSettingsForm
          ntpSettings={data}
          ntpSettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
        />
      </SectionContent>
    )
  }

}

export default restComponent(NTP_SETTINGS_ENDPOINT, NTPSettings);
