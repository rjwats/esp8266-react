import React, { Component } from 'react';
import { Redirect, Switch } from 'react-router-dom'

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import APSettings from '../containers/APSettings';
import APStatus from '../containers/APStatus';
import { withAuthenticationContext } from '../authentication/Context.js';

class AccessPoint extends Component {

  handleTabChange = (event, path) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticationContext } = this.props;
    return (
      <MenuAppBar sectionTitle="Access Point">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/ap/status" label="Access Point Status" />
          <Tab value="/ap/settings" label="Access Point Settings" disabled={!authenticationContext.isAdmin()} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact={true} path="/ap/status" component={APStatus} />
          <AuthenticatedRoute exact={true} path="/ap/settings" component={APSettings} />
          <Redirect to="/ap/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withAuthenticationContext(AccessPoint);
