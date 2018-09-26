import React, { Component } from 'react';

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import MenuAppBar from '../components/MenuAppBar';
import AudioLightFrequencyStatus from './AudioLightFrequencyStatus';
import AudioLightSettings from './AudioLightSettings';

class AudioLightControls extends Component {

  constructor(props) {
    super(props);

    this.state = {
        selectedTab: "audioLightFrequencyStatus",
    };
  }

  handleTabChange = (event, selectedTab) => {
    this.setState({ selectedTab });
  };

  render() {
    const { selectedTab } = this.state;
    return (
      <MenuAppBar sectionTitle="Audio Light Controls">
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" fullWidth scrollable>
           <Tab value="audioLightFrequencyStatus" label="Frequency Status" />
           <Tab value="audioLightSettings" label="Settings" />           
         </Tabs>
         {selectedTab === "audioLightFrequencyStatus" && <AudioLightFrequencyStatus />}
         {selectedTab === "audioLightSettings" && <AudioLightSettings />}
      </MenuAppBar>
    )
  }
  
}

export default AudioLightControls;
