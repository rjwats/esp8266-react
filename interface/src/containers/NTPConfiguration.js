import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import NTPSettings from './NTPSettings';
import NTPStatus from './NTPStatus';

import Tabs, { Tab } from 'material-ui/Tabs';

class NTPConfiguration extends Component {

  constructor(props) {
    super(props);
    this.state = {
        selectedTab: "ntpStatus"
    };
  }

  handleTabChange = (event, selectedTab) => {
    this.setState({ selectedTab });
  };

  render() {
    const { selectedTab } = this.state;
    return (
        <MenuAppBar sectionTitle="NTP Configuration">
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" fullWidth centered scrollable>
           <Tab value="ntpStatus" label="NTP Status" />
           <Tab value="ntpSettings" label="NTP Settings" />
         </Tabs>
         {selectedTab === "ntpStatus" && <NTPStatus />}
         {selectedTab === "ntpSettings" && <NTPSettings />}
        </MenuAppBar>
    )
  }
}

export default NTPConfiguration
