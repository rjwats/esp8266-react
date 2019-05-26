import React, { Component } from 'react';

import { Redirect, Switch } from 'react-router';

import * as Authentication from './authentication/Authentication';
import AuthenticationWrapper from './authentication/AuthenticationWrapper';
import AuthenticatedRoute from './authentication/AuthenticatedRoute';
import UnauthenticatedRoute from './authentication/UnauthenticatedRoute';

import SignInPage from './containers/SignInPage';

import WiFiConnection from './sections/WiFiConnection';
import AccessPoint from './sections/AccessPoint';
import NetworkTime from './sections/NetworkTime';
import Security from './sections/Security';
import System from './sections/System';

class AppRouting extends Component {

  componentWillMount() {
    Authentication.clearLoginRedirect();
  }

  render() {
    return (
      <AuthenticationWrapper>
        <Switch>
          <UnauthenticatedRoute exact path="/" component={SignInPage} />
          <AuthenticatedRoute exact path="/wifi/*" component={WiFiConnection} />
          <AuthenticatedRoute exact path="/ap/*" component={AccessPoint} />
          <AuthenticatedRoute exact path="/ntp/*" component={NetworkTime} />
          <AuthenticatedRoute exact path="/security/*" component={Security} />
          <AuthenticatedRoute exact path="/system/*" component={System} />
          <Redirect to="/" />
        </Switch>
      </AuthenticationWrapper>
    )
  }
}

export default AppRouting;
