import React, { Component } from 'react';

import { Redirect, Route, Switch } from 'react-router';

// authentication
import * as Authentication from './authentication/Authentication';
import AuthenticationWrapper from './authentication/AuthenticationWrapper';
import AuthenticatedRoute from './authentication/AuthenticatedRoute';

// containers
import WiFiConfiguration from './containers/WiFiConfiguration';
import NTPConfiguration from './containers/NTPConfiguration';
import OTAConfiguration from './containers/OTAConfiguration';
import APConfiguration from './containers/APConfiguration';
import LoginPage from './containers/LoginPage';

class AppRouting extends Component {

  componentWillMount() {
    Authentication.clearLoginRedirect();
  }

  render() {
    return (
      <AuthenticationWrapper>
        <Switch>
          <Route exact path="/" component={LoginPage} />
          <AuthenticatedRoute exact path="/wifi-configuration" component={WiFiConfiguration} />
          <AuthenticatedRoute exact path="/ap-configuration" component={APConfiguration} />
          <AuthenticatedRoute exact path="/ntp-configuration" component={NTPConfiguration} />
          <AuthenticatedRoute exact path="/ota-configuration" component={OTAConfiguration} />
          <Redirect to="/" />
        </Switch>
      </AuthenticationWrapper>
    )
  }
}

export default AppRouting;
