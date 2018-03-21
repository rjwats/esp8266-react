import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import LightStripSettings from './LightStripSettings';

class LightStripControls extends Component {
  render() {
    return (
        <MenuAppBar sectionTitle="Light Strip Controls">
          <LightStripSettings />
        </MenuAppBar>
    )
  }
}

export default LightStripControls
