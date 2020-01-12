import React, { Component } from 'react';
import { Switch, Redirect } from 'react-router';

import { PROJECT_PATH } from './constants/Env';

import * as Authentication from './authentication/Authentication';
import AuthenticationWrapper from './authentication/AuthenticationWrapper';
import UnauthenticatedRoute from './authentication/UnauthenticatedRoute';
import AuthenticatedRoute from './authentication/AuthenticatedRoute';

import SignInPage from './containers/SignInPage';
import NetworkTime from './sections/NetworkTime';
import System from './sections/System';
import AccessPoint from './sections/AccessPoint';
import Security from './sections/Security';
import ProjectRouting from './project/ProjectRouting';

/*
import { Redirect, Switch } from 'react-router';
import { PROJECT_PATH } from './constants/Env';

import AuthenticatedRoute from './authentication/AuthenticatedRoute';
import UnauthenticatedRoute from './authentication/UnauthenticatedRoute';
import SignInPage from './containers/SignInPage';
import WiFiConnection from './sections/WiFiConnection';
import AccessPoint from './sections/AccessPoint';
import NetworkTime from './sections/NetworkTime';
import Security from './sections/Security';
import System from './sections/System';
import ProjectRouting from './project/ProjectRouting';
*/

class AppRouting extends Component {

  componentDidMount() {
    Authentication.clearLoginRedirect();
  }

  render() {
    return (
      <AuthenticationWrapper>
        <Switch>
          <UnauthenticatedRoute exact path="/" component={SignInPage} />
          <AuthenticatedRoute exact path="/ntp/*" component={NetworkTime} />
          <AuthenticatedRoute exact path="/system/*" component={System} />
          <AuthenticatedRoute exact path="/ap/*" component={AccessPoint} />
          <AuthenticatedRoute exact path="/security/*" component={Security} /> 
          {
            // <AuthenticatedRoute exact path="/wifi/*" component={WiFiConnection} />         
          }
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/*`} component={ProjectRouting} />
          <Redirect to="/" />
        </Switch>
      </AuthenticationWrapper>
    )
  }
}

export default AppRouting;
