import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { SER2NET_SETTINGS_ENDPOINT } from '../api';

import Ser2netSettingsForm from './Ser2netSettingsForm';
import { Ser2netSettings } from './types';

type Ser2netSettingsControllerProps = RestControllerProps<Ser2netSettings>;

class Ser2netSettingsController extends Component<Ser2netSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Ser2net Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <Ser2netSettingsForm {...formProps} />}
        />
      </SectionContent>
    )
  }

}

export default restController(SER2NET_SETTINGS_ENDPOINT, Ser2netSettingsController);
