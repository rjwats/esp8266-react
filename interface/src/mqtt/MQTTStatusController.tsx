import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { MQTT_STATUS_ENDPOINT } from '../api';

import MQTTStatusForm from './MQTTStatusForm';
import { MQTTStatus } from './types';

type MQTTStatusControllerProps = RestControllerProps<MQTTStatus>;

class MQTTStatusController extends Component<MQTTStatusControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="MQTT Status">
        <RestFormLoader
          {...this.props}
          render={formProps => <MQTTStatusForm {...formProps} />}
        />
      </SectionContent>
    )
  }
}

export default restController(MQTT_STATUS_ENDPOINT, MQTTStatusController);
