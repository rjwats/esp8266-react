import React, { Component } from 'react';
import { Redirect, Switch } from 'react-router-dom'

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import NTPSettings from '../containers/NTPSettings';
import NTPStatus from '../containers/NTPStatus';
import { withAuthenticationContext } from '../authentication/Context.js';

class NetworkTime extends Component {

  handleTabChange = (event, path) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticationContext } = this.props;
    return (
      <MenuAppBar sectionTitle="Network Time">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/ntp/status" label="NTP Status" />
          <Tab value="/ntp/settings" label="NTP Settings" disabled={!authenticationContext.isAdmin()} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact={true} path="/ntp/status" component={NTPStatus} />
          <AuthenticatedRoute exact={true} path="/ntp/settings" component={NTPSettings} />
          <Redirect to="/ntp/status" />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default withAuthenticationContext(NetworkTime)
