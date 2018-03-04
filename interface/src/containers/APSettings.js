import React, { Component } from 'react';

import { AP_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import APSettingsForm from '../forms/APSettingsForm';

class APSettings extends Component {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="AP Settings">
      	<APSettingsForm          
          apSettings={data}
          apSettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
        />
      </SectionContent>
    )
  }

}

export default restComponent(AP_SETTINGS_ENDPOINT, APSettings);
