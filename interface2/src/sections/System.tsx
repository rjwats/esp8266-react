import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import OTASettings from '../containers/OTASettings';
import SystemStatus from '../containers/SystemStatus';
import { withAuthenticatedContext, AuthenticatedContextProps } from '../authentication/AuthenticationContext';

type SystemProps = AuthenticatedContextProps & RouteComponentProps;

class System extends Component<SystemProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticatedContext } = this.props;
    return (
      <MenuAppBar sectionTitle="System">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/system/status" label="System Status" />
          <Tab value="/system/ota" label="OTA Settings" disabled={!authenticatedContext.user.admin} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact={true} path="/system/status" component={SystemStatus} />
          <AuthenticatedRoute exact={true} path="/system/ota" component={OTASettings} />
          <Redirect to="/system/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withAuthenticatedContext(System);
