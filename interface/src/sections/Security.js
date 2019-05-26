import React, { Component } from 'react';
import { Redirect, Switch } from 'react-router-dom'

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import ManageUsers from '../containers/ManageUsers';
import SecuritySettings from '../containers/SecuritySettings';

class Security extends Component {

  handleTabChange = (event, path) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Security">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/security/users" label="Manage Users" />
          <Tab value="/security/settings" label="Security Settings" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact={true} path="/security/users" component={ManageUsers} />
          <AuthenticatedRoute exact={true} path="/security/settings" component={SecuritySettings} />
          <Redirect to="/security/users" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default Security;
