import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import APSettings from '../containers/APSettings';
import APStatus from '../containers/APStatus';
import { AuthenticatedContextProps, withAuthenticatedContext } from '../authentication/AuthenticationContext';

type AccessPointProps = AuthenticatedContextProps & RouteComponentProps;

class AccessPoint extends Component<AccessPointProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    const { authenticatedContext } = this.props;
    return (
      <MenuAppBar sectionTitle="Access Point">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/ap/status" label="Access Point Status" />
          <Tab value="/ap/settings" label="Access Point Settings" disabled={!authenticatedContext.user.admin} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact={true} path="/ap/status" component={APStatus} />
          <AuthenticatedRoute exact={true} path="/ap/settings" component={APSettings} />
          <Redirect to="/ap/status" />
        </Switch>
      </MenuAppBar>
    )
  }
}

export default withAuthenticatedContext(AccessPoint);
