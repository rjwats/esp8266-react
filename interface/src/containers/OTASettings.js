import React, { Component } from 'react';

import { OTA_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';
import SectionContent from '../components/SectionContent';
import OTASettingsForm from '../forms/OTASettingsForm';

class OTASettings extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { fetched, errorMessage, data, saveData, loadData, handleValueChange, handleCheckboxChange } = this.props;
    return (
      <SectionContent title="OTA Settings">
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={() =>
            <OTASettingsForm
              otaSettings={data}
              onSubmit={saveData}
              onReset={loadData}
              handleValueChange={handleValueChange}
              handleCheckboxChange={handleCheckboxChange}
            />
          }
        />
      </SectionContent>
    )
  }

}

export default restComponent(OTA_SETTINGS_ENDPOINT, OTASettings);
