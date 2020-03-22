import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { MQTT_SETTINGS_ENDPOINT } from '../api';

import MQTTSettingsForm from './MQTTSettingsForm';
import { MQTTSettings } from './types';

type MQTTSettingsControllerProps = RestControllerProps<MQTTSettings>;

class MQTTSettingsController extends Component<MQTTSettingsControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="MQTT Settings" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <MQTTSettingsForm {...formProps} />}
        />
      </SectionContent>
    )
  }

}

export default restController(MQTT_SETTINGS_ENDPOINT, MQTTSettingsController);
