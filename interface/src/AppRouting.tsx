import React, { Component } from 'react';
import { Switch, Redirect } from 'react-router';

import * as Authentication from './authentication/Authentication';
import AuthenticationWrapper from './authentication/AuthenticationWrapper';
import UnauthenticatedRoute from './authentication/UnauthenticatedRoute';
import AuthenticatedRoute from './authentication/AuthenticatedRoute';

import SignIn from './SignIn';
import ProjectRouting from './project/ProjectRouting';
import WiFiConnection from './wifi/WiFiConnection';
import AccessPoint from './ap/AccessPoint';
import NetworkTime from './ntp/NetworkTime';
import Security from './security/Security';
import System from './system/System';

import { PROJECT_PATH } from './api';
import Mqtt from './mqtt/Mqtt';

class AppRouting extends Component {

  componentDidMount() {
    Authentication.clearLoginRedirect();
  }

  render() {
    return (
      <AuthenticationWrapper>
        <Switch>
          <UnauthenticatedRoute exact path="/" component={SignIn} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/*`} component={ProjectRouting} />
          <AuthenticatedRoute exact path="/wifi/*" component={WiFiConnection} />         
          <AuthenticatedRoute exact path="/ap/*" component={AccessPoint} />
          <AuthenticatedRoute exact path="/ntp/*" component={NetworkTime} />
          <AuthenticatedRoute exact path="/mqtt/*" component={Mqtt} />
          <AuthenticatedRoute exact path="/security/*" component={Security} /> 
          <AuthenticatedRoute exact path="/system/*" component={System} />          
          <Redirect to="/" />
        </Switch>
      </AuthenticationWrapper>
    )
  }
}

export default AppRouting;
