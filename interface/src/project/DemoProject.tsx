import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import DemoInformation from './DemoInformation';
import LightStateRestController from './LightStateRestController';
import LightStateWebSocketController from './LightStateWebSocketController';
import LightMqttSettingsController from './LightMqttSettingsController';
import { WrappedComponentProps, injectIntl } from 'react-intl';

type DemoProjectProps = RouteComponentProps & WrappedComponentProps;

class DemoProject extends Component<DemoProjectProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    const { intl } = this.props;
    return (
      <MenuAppBar sectionTitle="Demo Project">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/demo/information`} label={intl.formatMessage({
            id: 'project.information',
            defaultMessage: 'Information'
          })} />
          <Tab value={`/${PROJECT_PATH}/demo/rest`} label={intl.formatMessage({
            id: 'project.restController',
            defaultMessage: 'REST Controller'
          })} />
          <Tab value={`/${PROJECT_PATH}/demo/socket`} label={intl.formatMessage({
            id: 'project.webSocketController',
            defaultMessage: 'WebSocket Controller'
          })} />
          <Tab value={`/${PROJECT_PATH}/demo/mqtt`} label={intl.formatMessage({
            id: 'project.mqttController',
            defaultMessage: 'MQTT Controller'
          })} />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/information`} component={DemoInformation} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/rest`} component={LightStateRestController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/socket`} component={LightStateWebSocketController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/mqtt`} component={LightMqttSettingsController} />
          <Redirect to={`/${PROJECT_PATH}/demo/information`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default injectIntl(DemoProject);
