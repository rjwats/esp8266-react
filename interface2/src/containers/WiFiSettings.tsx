import React, { Component } from 'react';

import { WIFI_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import SectionContent from '../components/SectionContent';
import WiFiSettingsForm from '../forms/WiFiSettingsForm';
import { RestControllerProps, restController } from '../components/RestController';
import { WiFiConnectionContext } from '../sections/WiFiConnectionContext';
import RestFormLoader from '../components/RestFormLoader';

export interface WiFiSettingsData {
  ssid: string;
  password: string;
  hostname: string;
  static_ip_config: boolean;
  local_ip?: string;
  gateway_ip?: string;
  subnet_mask?: string;
  dns_ip_1?: string;
  dns_ip_2?: string;
}

type WiFiSettingsProps = RestControllerProps<WiFiSettingsData>;

class WiFiSettings extends Component<WiFiSettingsProps> {

  static contextType = WiFiConnectionContext;
  context!: React.ContextType<typeof WiFiConnectionContext>;

  componentDidMount() {
    const { selectedNetwork } = this.context;
    if (selectedNetwork) {
      const wifiSettings: WiFiSettingsData = {
        ssid: selectedNetwork.ssid,
        password: "",
        hostname: "esp8266-react",
        static_ip_config: false,
      }
      this.props.setData(wifiSettings);
    } else {
      this.props.loadData();
    }
  }

  deselectNetworkAndLoadData = () => {
    this.context.deselectNetwork();
    this.props.loadData();
  }

  componentWillUnmount() {
    this.context.deselectNetwork();
  }

  render() {
    return (
      <SectionContent title="WiFi Settings">
        <RestFormLoader
          {...this.props}
          loadData={this.deselectNetworkAndLoadData}
          render={formProps => <WiFiSettingsForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(WIFI_SETTINGS_ENDPOINT, WiFiSettings);
