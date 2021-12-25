import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { AuthenticatedContextProps, withAuthenticatedContext, AuthenticatedRoute } from '../authentication';
import { MenuAppBar } from '../components';
import SerialStatusController from './SerialStatusController';
import SerialSettingsController from './SerialSettingsController';
import LogEventController from './LogEventController';

type SerialProps = AuthenticatedContextProps & RouteComponentProps;

class Serial extends Component<SerialProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticatedContext } = this.props;
    return (
      <MenuAppBar sectionTitle="Serial">
        <Tabs id="serial-tabs" value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value="/serial/status" label="Serial Status" />
          <Tab value="/serial/log" label="Remote Log" />
          <Tab value="/serial/settings" label="Serial Settings" disabled={!authenticatedContext.me.admin} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path="/serial/status" component={SerialStatusController} />
          <AuthenticatedRoute exact path="/serial/log" component={LogEventController} />
          <AuthenticatedRoute exact path="/serial/settings" component={SerialSettingsController} />
          <Redirect to="/serial/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withAuthenticatedContext(Serial);
