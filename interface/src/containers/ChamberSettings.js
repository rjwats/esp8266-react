import React, { Component } from 'react';

import { CHAMBER_SETTINGS_ENDPOINT, CHAMBER_STATUS_ENDPOINT  }  from  '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import ChamberSettingsForm from '../forms/ChamberSettingsForm';

class ChamberSettings extends Component {

  render() {
    const {
      data, fetched, errorMessage,
      chamberStatus, chamberStatusFetched, chamberStatusErrorMessage } = this.props;
    return (
      <SectionContent title="Chamber Settings">
      	<ChamberSettingsForm
          chamberSettings={data}
          chamberStatus={chamberStatus}
          fetched={fetched || chamberStatusFetched}
          errorMessage={errorMessage || chamberStatusErrorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
          handleCheckboxChange={this.props.handleCheckboxChange}
        />
      </SectionContent>
    )
  }

}

const ComponentWithSettings = restComponent(CHAMBER_SETTINGS_ENDPOINT, ChamberSettings);

class ChamberSettingsWithStatus extends Component {

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      	<ComponentWithSettings
          chamberStatus={data}
          chamberStatusFetched={fetched}
          chamberStatusErrorMessage={errorMessage}
        />
    )
  }

}


export default restComponent(CHAMBER_STATUS_ENDPOINT, ChamberSettingsWithStatus);
