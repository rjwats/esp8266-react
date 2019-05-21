import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import ManageUsers from './ManageUsers';

class UserConfiguration extends Component {
  render() {
    return (
        <MenuAppBar sectionTitle="User Configuration">
          <ManageUsers />
        </MenuAppBar>
    )
  }
}

export default UserConfiguration
