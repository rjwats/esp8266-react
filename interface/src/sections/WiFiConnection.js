import React, { Component } from 'react';
import { Redirect, Switch } from 'react-router-dom'

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import WiFiNetworkScanner from '../containers/WiFiNetworkScanner';
import WiFiSettings from '../containers/WiFiSettings';
import WiFiStatus from '../containers/WiFiStatus';
import { withAuthenticationContext } from '../authentication/Context.js';

class WiFiConnection extends Component {

  constructor(props) {
    super(props);
    this.state = {
      selectedNetwork: null
    };
    this.selectNetwork = this.selectNetwork.bind(this);
    this.deselectNetwork = this.deselectNetwork.bind(this);
  }

  selectNetwork(network) {
    this.setState({ selectedNetwork: network });
    this.props.history.push('/wifi/settings');
  }

  deselectNetwork(network) {
    this.setState({ selectedNetwork: null });
  }

  handleTabChange = (event, path) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticationContext } = this.props;
    const ConfiguredWiFiNetworkScanner = (props) => {
      return (
        <WiFiNetworkScanner
          selectNetwork={this.selectNetwork}
          {...props}
        />
      );
    };
    const ConfiguredWiFiSettings = (props) => {
      return (
        <WiFiSettings
          deselectNetwork={this.deselectNetwork} selectedNetwork={this.state.selectedNetwork}
          {...props}
        />
      );
    };
    return (
      <MenuAppBar sectionTitle="WiFi Connection">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/wifi/status" label="WiFi Status" />
          <Tab value="/wifi/scan" label="Scan Networks" disabled={!authenticationContext.isAdmin()} />
          <Tab value="/wifi/settings" label="WiFi Settings" disabled={!authenticationContext.isAdmin()} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact={true} path="/wifi/status" component={WiFiStatus} />
          <AuthenticatedRoute exact={true} path="/wifi/scan" component={ConfiguredWiFiNetworkScanner} />
          <AuthenticatedRoute exact={true} path="/wifi/settings" component={ConfiguredWiFiSettings} />
          <Redirect to="/wifi/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withAuthenticationContext(WiFiConnection);
