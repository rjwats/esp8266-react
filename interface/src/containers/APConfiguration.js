import React, { Component } from 'react';

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import MenuAppBar from '../components/MenuAppBar';
import APSettings from './APSettings';
import APStatus from './APStatus';

class APConfiguration extends Component {

  constructor(props) {
    super(props);
    this.state = {
        selectedTab: "apStatus",
        selectedNetwork: null
    };
  }

  handleTabChange = (event, selectedTab) => {
    this.setState({ selectedTab });
  };

  render() {
    const { selectedTab } = this.state;
    return (
      <MenuAppBar sectionTitle="AP Configuration">
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" fullWidth scrollable>
           <Tab value="apStatus" label="AP Status" />
           <Tab value="apSettings" label="AP Settings" />
         </Tabs>
         {selectedTab === "apStatus" && <APStatus fullDetails={true} />}
         {selectedTab === "apSettings" && <APSettings />}
      </MenuAppBar>
    )
  }
}

export default APConfiguration;
