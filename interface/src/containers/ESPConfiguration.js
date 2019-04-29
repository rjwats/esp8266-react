import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import ESPStatus from './ESPStatus';

class ESPConfiguration extends Component {
  render() {
    return (
        <MenuAppBar sectionTitle="ESP Configuration">
          <ESPStatus />
        </MenuAppBar>
    )
  }
}

export default ESPConfiguration
