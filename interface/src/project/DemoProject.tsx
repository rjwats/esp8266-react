import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import StateWebSocketController from './StateWebSocketController';
import MqttSettingsController from './MqttSettingsController';

class DemoProject extends Component<RouteComponentProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Demo Project">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/demo/socket`} label="Controller" />
          <Tab value={`/${PROJECT_PATH}/demo/mqtt`} label="MQTT Controller" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/socket`} component={StateWebSocketController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/mqtt`} component={MqttSettingsController} />
          <Redirect to={`/${PROJECT_PATH}/demo/socket`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default DemoProject;
