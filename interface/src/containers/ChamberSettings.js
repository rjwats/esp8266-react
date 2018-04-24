import React, { Component } from 'react';

import { CHAMBER_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import ChamberSettingsForm from '../forms/ChamberSettingsForm';

class ChamberSettings extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="AP Settings">
      	<ChamberSettingsForm
          chamberSettings={data}
          chamberSettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
        />
      </SectionContent>
    )
  }

}

export default restComponent(CHAMBER_SETTINGS_ENDPOINT, ChamberSettings);
