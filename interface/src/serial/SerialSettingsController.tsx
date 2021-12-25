import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { SERIAL_SETTINGS_ENDPOINT } from '../api';

import SerialSettingsForm from './SerialSettingsForm';
import { SerialSettings } from './types';

type SerialSettingsControllerProps = RestControllerProps<SerialSettings>;

class SerialSettingsController extends Component<SerialSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Serial Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <SerialSettingsForm {...formProps} />}
        />
      </SectionContent>
    )
  }

}

export default restController(SERIAL_SETTINGS_ENDPOINT, SerialSettingsController);
