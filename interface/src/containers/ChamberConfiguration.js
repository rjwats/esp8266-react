import React, { Component } from 'react';

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import MenuAppBar from '../components/MenuAppBar';
import ChamberSettings from './ChamberSettings';
import ChamberStatus from './ChamberStatus';
import ChamberLog from './ChamberLog';

class ChamberConfiguration extends Component {

  constructor(props) {
    super(props);
    this.state = {
        selectedTab: "chamberStatus",
        selectedNetwork: null
    };
  }

  handleTabChange = (event, selectedTab) => {
    this.setState({ selectedTab });
  };

  render() {
    const { selectedTab } = this.state;
    return (
      <MenuAppBar sectionTitle="Chamber Configuration">
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" fullWidth centered scrollable>
           <Tab value="chamberStatus" label="Chamber Status" />
           <Tab value="chamberLog" label="Chamber Log" />
           <Tab value="chamberSettings" label="Chamber Settings" />
         </Tabs>
         {selectedTab === "chamberStatus" && <ChamberStatus />}
         {selectedTab === "chamberLog" && <ChamberLog />}
         {selectedTab === "chamberSettings" && <ChamberSettings />}
      </MenuAppBar>
    )
  }
}

export default ChamberConfiguration;
