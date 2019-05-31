import React, { Component } from 'react';
import { Redirect, Switch } from 'react-router-dom'

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import OTASettings from '../containers/OTASettings';
import SystemStatus from '../containers/SystemStatus';
import { withAuthenticationContext } from '../authentication/Context.js';

class System extends Component {

  handleTabChange = (event, path) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticationContext } = this.props;
    return (
      <MenuAppBar sectionTitle="System">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/system/status" label="System Status" />
          <Tab value="/system/ota" label="OTA Settings" disabled={!authenticationContext.isAdmin()} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact={true} path="/system/status" component={SystemStatus} />
          <AuthenticatedRoute exact={true} path="/system/ota" component={OTASettings} />
          <Redirect to="/system/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withAuthenticationContext(System);
