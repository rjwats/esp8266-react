import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { SER2NET_STATUS_ENDPOINT } from '../api';

import Ser2netStatusForm from './Ser2netStatusForm';
import { Ser2netStatus } from './types';

type Ser2netStatusControllerProps = RestControllerProps<Ser2netStatus>;

class Ser2netStatusController extends Component<Ser2netStatusControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Ser2net Status">
        <RestFormLoader
          {...this.props}
          render={formProps => <Ser2netStatusForm {...formProps} />}
        />
      </SectionContent>
    )
  }
}

export default restController(SER2NET_STATUS_ENDPOINT, Ser2netStatusController);