import React, { Component } from 'react';

import MenuAppBar from '../components/MenuAppBar';
import Bme280Controller from './Bme280Controller';

class Bme280Project extends Component {

  handleTabChange = (event, path) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="BME280">
        <Bme280Controller />
      </MenuAppBar>
    )
  }        

}

export default Bme280Project;
