import React, { Component } from 'react';
import PropTypes from 'prop-types';

import { WIFI_SETTINGS_ENDPOINT } from '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';
import SectionContent from '../components/SectionContent';
import WiFiSettingsForm from '../forms/WiFiSettingsForm';

class WiFiSettings extends Component {

  constructor(props) {
    super(props);

    this.deselectNetworkAndLoadData = this.deselectNetworkAndLoadData.bind(this);
  }

  componentDidMount() {
    const { selectedNetwork } = this.props;
    if (selectedNetwork) {
      var wifiSettings = {
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

  deselectNetworkAndLoadData() {
    this.props.deselectNetwork();
    this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage, saveData, loadData, handleValueChange, handleCheckboxChange, selectedNetwork, deselectNetwork } = this.props;
    return (
      <SectionContent title="WiFi Settings">
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={() =>
            <WiFiSettingsForm
              wifiSettings={data}
              selectedNetwork={selectedNetwork}
              deselectNetwork={deselectNetwork}
              onSubmit={saveData}
              onReset={this.deselectNetworkAndLoadData}
              handleValueChange={handleValueChange}
              handleCheckboxChange={handleCheckboxChange}
            />
          }
        />
      </SectionContent>
    )
  }

}

WiFiSettings.propTypes = {
  deselectNetwork: PropTypes.func,
  selectedNetwork: PropTypes.object
};

export default restComponent(WIFI_SETTINGS_ENDPOINT, WiFiSettings);
