import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import NTPStatusForm from '../forms/NTPStatusForm';
import { NTP_STATUS_ENDPOINT } from '../constants/Endpoints';

export interface NTPStatus {
  status: number;
  time_utc: string;
  time_local: string;
  server: string;
  uptime: number;
}

type NTPStatusControllerProps = RestControllerProps<NTPStatus>;

class NTPStatusController extends Component<NTPStatusControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="NTP Status">
        <RestFormLoader
          {...this.props}
          render={formProps => <NTPStatusForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(NTP_STATUS_ENDPOINT, NTPStatusController);
