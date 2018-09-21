import React, { Component } from 'react';

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import MenuAppBar from '../components/MenuAppBar';
import WiFiNetworkScanner from './WiFiNetworkScanner';
import WiFiSettings from './WiFiSettings';
import WiFiStatus from './WiFiStatus';

class WiFiConfiguration extends Component {

  constructor(props) {
    super(props);
    this.state = {
        selectedTab: "wifiStatus",
        selectedNetwork: null
    };
    this.selectNetwork = this.selectNetwork.bind(this);
    this.deselectNetwork = this.deselectNetwork.bind(this);
  }

  // TODO - slightly inapproperate use of callback ref possibly.
  selectNetwork(network) {
    this.setState({ selectedTab: "wifiSettings", selectedNetwork:network });
  }

  // deselects the network after the settings component mounts.
  deselectNetwork(network) {
    this.setState({ selectedNetwork:null });
  }

  handleTabChange = (event, selectedTab) => {
    this.setState({ selectedTab });
  };

  render() {
    const { selectedTab } = this.state;
    return (
      <MenuAppBar sectionTitle="WiFi Configuration">
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" fullWidth scrollable>
           <Tab value="wifiStatus" label="WiFi Status" />
           <Tab value="networkScanner" label="Network Scanner" />
           <Tab value="wifiSettings" label="WiFi Settings" />
         </Tabs>
         {selectedTab === "wifiStatus" && <WiFiStatus fullDetails={true} />}
         {selectedTab === "networkScanner" && <WiFiNetworkScanner selectNetwork={this.selectNetwork} />}
         {selectedTab === "wifiSettings" && <WiFiSettings deselectNetwork={this.deselectNetwork} selectedNetwork={this.state.selectedNetwork} />}
      </MenuAppBar>
    )
  }
}

export default WiFiConfiguration;
