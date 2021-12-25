import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { SERIAL_STATUS_ENDPOINT } from '../api';

import SerialStatusForm from './SerialStatusForm';
import { SerialStatus } from './types';

type SerialStatusControllerProps = RestControllerProps<SerialStatus>;

class SerialStatusController extends Component<SerialStatusControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Serial Status">
        <RestFormLoader
          {...this.props}
          render={formProps => <SerialStatusForm {...formProps} />}
        />
      </SectionContent>
    )
  }
}

export default restController(SERIAL_STATUS_ENDPOINT, SerialStatusController);