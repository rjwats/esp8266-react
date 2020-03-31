import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import DemoInformation from './DemoInformation';
import DemoController from './DemoController';
import DemoSocketController from './DemoSocketController';

class DemoProject extends Component<RouteComponentProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Demo Project">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/demo/information`} label="Information" />
          <Tab value={`/${PROJECT_PATH}/demo/controller`} label="Rest Controller" />
          <Tab value={`/${PROJECT_PATH}/demo/socket`} label="Socket Controller" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/information`} component={DemoInformation} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/controller`} component={DemoController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/socket`} component={DemoSocketController} />
          <Redirect to={`/${PROJECT_PATH}/demo/information`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default DemoProject;
