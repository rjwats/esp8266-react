import React, { Component } from 'react';

import SectionContent from '../components/SectionContent';
import { WIFI_STATUS_ENDPOINT } from '../constants/Endpoints';
import { restController, RestControllerProps } from '../components/RestController';
import RestFormLoader from '../components/RestFormLoader';
import WiFiStatusForm from '../forms/WiFiStatusForm';

export interface WiFiStatus {
  status: number;
  local_ip: string;
  mac_address: string;
  rssi: number;
  ssid: string;
  bssid: string;
  channel: number;
  subnet_mask: string;
  gateway_ip: string;
  dns_ip_1: string;
  dns_ip_2: string;
}

type WiFiStatusControllerProps = RestControllerProps<WiFiStatus>;

class WiFiStatusController extends Component<WiFiStatusControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="WiFi Status">
        <RestFormLoader
          {...this.props}
          render={formProps => <WiFiStatusForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(WIFI_STATUS_ENDPOINT, WiFiStatusController);
