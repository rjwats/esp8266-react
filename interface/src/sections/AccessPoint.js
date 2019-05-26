import React, { Component } from 'react';
import { Redirect, Switch } from 'react-router-dom'

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import MenuAppBar from '../components/MenuAppBar';
import APSettings from '../containers/APSettings';
import APStatus from '../containers/APStatus';

class AccessPoint extends Component {

  handleTabChange = (event, path) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="AP Configuration">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value="/ap/status" label="AP Status" />
          <Tab value="/ap/settings" label="AP Settings" />
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

export default AccessPoint;
