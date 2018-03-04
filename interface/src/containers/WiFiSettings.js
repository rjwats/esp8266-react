import React, { Component } from 'react';
import PropTypes from 'prop-types';

import { WIFI_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import { restComponent } from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import WiFiSettingsForm from '../forms/WiFiSettingsForm';
import { simpleGet }  from  '../helpers/SimpleGet';
import { simplePost } from '../helpers/SimplePost';

class WiFiSettings extends Component {

  constructor(props) {
    super(props);

    this.state = {
             selectedNetwork: null,
           };
    this.deselectNetworkAndLoadData = this.deselectNetworkAndLoadData.bind(this);
    this.deselectNetwork = this.deselectNetwork.bind(this);
  }

  componentDidMount() {
    const { selectedNetwork, deselectNetwork } = this.props;
    if (selectedNetwork){
      var wifiSettings = {
        ssid:selectedNetwork.ssid,
        password:"",
        hostname:"esp8266-react",
        static_ip_config:false,
      }
      this.props.setData(wifiSettings);
      this.setState({ selectedNetwork });
      deselectNetwork();
    }else {
      this.props.loadData();
    }
  }

  deselectNetworkAndLoadData() {
    this.deselectNetwork();
    this.props.loadData();
  }

  deselectNetwork() {
    this.setState({ selectedNetwork:null });
  }

  render() {
    const { selectedNetwork } = this.state;
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="WiFi Settings">
      	<WiFiSettingsForm
          wifiSettings={data}
          wifiSettingsFetched={fetched}
          errorMessage={errorMessage}
          selectedNetwork={selectedNetwork}
          deselectNetwork={this.deselectNetwork}
          onSubmit={this.props.saveData}
          onReset={this.deselectNetworkAndLoadData}
          handleValueChange={this.props.handleValueChange}
          handleCheckboxChange={this.props.handleCheckboxChange}
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
