import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import OTASettings from './OTASettings';

class OTAConfiguration extends Component {
  render() {
    return (
        <MenuAppBar sectionTitle="OTA Configuration">
          <OTASettings />
        </MenuAppBar>
    )
  }
}

export default OTAConfiguration
