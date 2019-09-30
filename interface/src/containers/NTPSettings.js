import React, { Component } from 'react';

import { NTP_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';
import SectionContent from '../components/SectionContent';
import NTPSettingsForm from '../forms/NTPSettingsForm';

class NTPSettings extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { fetched, errorMessage, data, saveData, loadData, handleValueChange } = this.props;
    return (
      <SectionContent title="NTP Settings">
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={() =>
            <NTPSettingsForm
              ntpSettings={data}
              onSubmit={saveData}
              onReset={loadData}
              handleValueChange={handleValueChange}
            />
          }
        />
      </SectionContent>
    )
  }

}

export default restComponent(NTP_SETTINGS_ENDPOINT, NTPSettings);
