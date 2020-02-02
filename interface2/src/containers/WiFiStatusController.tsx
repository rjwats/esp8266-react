import React, { Component } from 'react';

import {restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import WiFiStatusForm from '../forms/WiFiStatusForm';
import { WIFI_STATUS_ENDPOINT } from '../constants/Endpoints';

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
