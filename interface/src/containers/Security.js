import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import ManageUsers from './ManageUsers';

class Security extends Component {
  render() {
    return (
        <MenuAppBar sectionTitle="Security">
          <ManageUsers />
        </MenuAppBar>
    )
  }
}

export default Security
