import React, { Component } from 'react';
import PropTypes from 'prop-types';

import { WIFI_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import {withNotifier} from '../components/SnackbarNotification';
import SectionContent from '../components/SectionContent';
import WiFiSettingsForm from '../forms/WiFiSettingsForm';
import { simpleGet }  from  '../helpers/SimpleGet';
import { simplePost } from '../helpers/SimplePost';

class WiFiSettings extends Component {

  constructor(props) {
    super(props);

    this.state = {
             wifiSettingsFetched: false,
             wifiSettings:{},
             selectedNetwork: null,
             errorMessage:null
           };

    this.setState = this.setState.bind(this);
    this.loadWiFiSettings = this.loadWiFiSettings.bind(this);
    this.saveWiFiSettings = this.saveWiFiSettings.bind(this);
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
      this.setState({wifiSettingsFetched:true, wifiSettings, selectedNetwork, errorMessage:null});
      deselectNetwork();
    }else {
      this.loadWiFiSettings();
    }
  }

  loadWiFiSettings() {
    this.deselectNetwork();

    simpleGet(
      WIFI_SETTINGS_ENDPOINT,
      this.setState,
      this.props.raiseNotification,
      "wifiSettings",
      "wifiSettingsFetched"
    );
  }

  saveWiFiSettings(e) {
    simplePost(
      WIFI_SETTINGS_ENDPOINT,
      this.state,
      this.setState,
      this.props.raiseNotification,
      "wifiSettings",
      "wifiSettingsFetched"
    );
  }

  deselectNetwork(nextNetwork) {
    this.setState({selectedNetwork:null});
  }

  wifiSettingValueChange = name => event => {
    const { wifiSettings } = this.state;
    wifiSettings[name] = event.target.value;
    this.setState({wifiSettings});
  };

  wifiSettingCheckboxChange = name => event => {
    const { wifiSettings } = this.state;
    wifiSettings[name] = event.target.checked;
    this.setState({wifiSettings});
  }

  render() {
    const { wifiSettingsFetched, wifiSettings, errorMessage, selectedNetwork } = this.state;
    return (
      <SectionContent title="WiFi Settings">
      	<WiFiSettingsForm wifiSettingsFetched={wifiSettingsFetched} wifiSettings={wifiSettings} errorMessage={errorMessage} selectedNetwork={selectedNetwork} deselectNetwork={this.deselectNetwork}
          onSubmit={this.saveWiFiSettings} onReset={this.loadWiFiSettings} handleValueChange={this.wifiSettingValueChange} handleCheckboxChange={this.wifiSettingCheckboxChange} />
      </SectionContent>
    )
  }

}

WiFiSettings.propTypes = {
  deselectNetwork: PropTypes.func,
  selectedNetwork: PropTypes.object
};

export default withNotifier(WiFiSettings);
