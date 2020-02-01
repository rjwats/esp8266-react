import React, { Component } from 'react';

import { NTP_STATUS_ENDPOINT } from '../constants/Endpoints';
import SectionContent from '../components/SectionContent';
import { RestControllerProps, restController } from '../components/RestController';
import RestFormLoader from '../components/RestFormLoader';
import NTPStatusForm from '../forms/NTPStatusForm';

export interface NTPStatusData {
  status: number;
  time_utc: string;
  time_local: string;
  server: string;
  uptime: number;
}

type NTPStatusProps = RestControllerProps<NTPStatusData>;

class NTPStatus extends Component<NTPStatusProps> {

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

export default restController(NTP_STATUS_ENDPOINT, NTPStatus);
