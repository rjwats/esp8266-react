import React, { Component } from 'react';

import RestFormLoader from '../components/RestFormLoader';
import SectionContent from '../components/SectionContent';
import { restController, RestControllerProps } from '../components/RestController';
import SystemStatusForm from '../forms/SystemStatusForm';
import { SYSTEM_STATUS_ENDPOINT } from '../constants/Endpoints';

export interface SystemStatusData {
  esp_platform: string;
  cpu_freq_mhz: number;
  free_heap: number;
  sketch_size: number;
  free_sketch_space: number;
  flash_chip_size: number;
}

type SystemStatusProps = RestControllerProps<SystemStatusData>;

class SystemStatus extends Component<SystemStatusProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="System Status">
        <RestFormLoader
          {...this.props}
          render={formProps => <SystemStatusForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(SYSTEM_STATUS_ENDPOINT, SystemStatus);
