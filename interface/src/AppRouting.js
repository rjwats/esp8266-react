import React, { Component } from 'react';

import { Redirect, Switch } from 'react-router';

// authentication
import * as Authentication from './authentication/Authentication';
import AuthenticationWrapper from './authentication/AuthenticationWrapper';
import AuthenticatedRoute from './authentication/AuthenticatedRoute';
import UnauthenticatedRoute from './authentication/UnauthenticatedRoute';

// containers
import WiFiConfiguration from './containers/WiFiConfiguration';
import NTPConfiguration from './containers/NTPConfiguration';
import OTAConfiguration from './containers/OTAConfiguration';
import APConfiguration from './containers/APConfiguration';
import SignInPage from './containers/SignInPage';
import Security from './containers/Security';

class AppRouting extends Component {

  componentWillMount() {
    Authentication.clearLoginRedirect();
  }

  render() {
    return (
      <AuthenticationWrapper>
        <Switch>
          <UnauthenticatedRoute exact path="/" component={SignInPage} />
          <AuthenticatedRoute exact path="/wifi-configuration" component={WiFiConfiguration} />
          <AuthenticatedRoute exact path="/ap-configuration" component={APConfiguration} />
          <AuthenticatedRoute exact path="/ntp-configuration" component={NTPConfiguration} />
          <AuthenticatedRoute exact path="/ota-configuration" component={OTAConfiguration} />
          <AuthenticatedRoute exact path="/security/*" component={Security} />
          <Redirect to="/" />
        </Switch>
      </AuthenticationWrapper>
    )
  }
}

export default AppRouting;
