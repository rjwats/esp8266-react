import React, { Component } from 'react';

import RestFormLoader from '../components/RestFormLoader';
import SectionContent from '../components/SectionContent';
import { restController, RestControllerProps} from '../components/RestController';
import APSettingsForm from '../forms/APSettingsForm';
import { AP_SETTINGS_ENDPOINT } from '../constants/Endpoints';

export interface APSettingsData {
  provision_mode: number;
  ssid: string;
  password: string;  
}

class APSettings extends Component<RestControllerProps<APSettingsData>> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="AP Settings">
        <RestFormLoader
          {...this.props}
          render={formProps => <APSettingsForm {...formProps} />}
        />
      </SectionContent>
    )
  }
  
}

export default restController(AP_SETTINGS_ENDPOINT, APSettings);
