import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import WiFiStatus from '../containers/WiFiStatus';
import { withAuthenticatedContext, AuthenticatedContextProps } from '../authentication/AuthenticationContext';

type WiFiConnectionProps = AuthenticatedContextProps & RouteComponentProps;

class WiFiConnection extends Component<WiFiConnectionProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    // const { authenticatedContext } = this.props;
    return (
      <MenuAppBar sectionTitle="WiFi Connection">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/wifi/status" label="WiFi Status" />
          {
            // <Tab value="/wifi/scan" label="Scan Networks" disabled={!authenticationContext.isAdmin()} />
            // <Tab value="/wifi/settings" label="WiFi Settings" disabled={!authenticationContext.isAdmin()} />
          }
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact={true} path="/wifi/status" component={WiFiStatus} />
          {
            // <AuthenticatedRoute exact={true} path="/wifi/scan" component={ConfiguredWiFiNetworkScanner} />
            // <AuthenticatedRoute exact={true} path="/wifi/settings" component={ConfiguredWiFiSettings} />
          }
          <Redirect to="/wifi/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withAuthenticatedContext(WiFiConnection);
