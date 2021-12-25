import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { AuthenticatedContextProps, withAuthenticatedContext, AuthenticatedRoute } from '../authentication';
import { MenuAppBar } from '../components';
import Ser2netStatusController from './Ser2netStatusController';
import Ser2netSettingsController from './Ser2netSettingsController';

type Ser2netProps = AuthenticatedContextProps & RouteComponentProps;

class Ser2net extends Component<Ser2netProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticatedContext } = this.props;
    return (
      <MenuAppBar sectionTitle="Ser2net">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value="/ser2net/status" label="Ser2net Status" />
          <Tab value="/ser2net/settings" label="Ser2net Settings" disabled={!authenticatedContext.me.admin} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path="/ser2net/status" component={Ser2netStatusController} />
          <AuthenticatedRoute exact path="/ser2net/settings" component={Ser2netSettingsController} />
          <Redirect to="/ser2net/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withAuthenticatedContext(Ser2net);
