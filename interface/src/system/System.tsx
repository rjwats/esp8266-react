import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { withAuthenticatedContext, AuthenticatedContextProps, AuthenticatedRoute } from '../authentication';
import { MenuAppBar } from '../components';

import SystemStatusController from './SystemStatusController';
import OTASettingsController from './OTASettingsController';
import { WithFeaturesProps, withFeatures } from '../features/FeaturesContext';

type SystemProps = AuthenticatedContextProps & RouteComponentProps & WithFeaturesProps;

class System extends Component<SystemProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticatedContext, features } = this.props;
    return (
      <MenuAppBar sectionTitle="System">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value="/system/status" label="System Status" />
          {features.ota && (
            <Tab value="/system/ota" label="OTA Settings" disabled={!authenticatedContext.me.admin} />
          )}
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path="/system/status" component={SystemStatusController} />
          {features.ota && (
            <AuthenticatedRoute exact path="/system/ota" component={OTASettingsController} />
          )}
          <Redirect to="/system/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withFeatures(withAuthenticatedContext(System));
