import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../../api';
import { AuthenticatedRoute } from '../../authentication';
import { MenuAppBar } from '../../components';

import ChamberSettingsController from './ChamberSettingsController';
import ChamberLogController from './ChamberLogController';
import ChamberStatusController from './ChamberStatusController';

class Chamber extends Component<RouteComponentProps>  {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Fermentation Chamber">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/chamber/status`} label="Chamber Status" />
          <Tab value={`/${PROJECT_PATH}/chamber/log`} label="Chamber Log" />
          <Tab value={`/${PROJECT_PATH}/chamber/settings`} label="Chamber Settings" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/chamber/status`} component={ChamberStatusController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/chamber/log`} component={ChamberLogController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/chamber/settings`} component={ChamberSettingsController} />
          <Redirect to={`/${PROJECT_PATH}/chamber/status`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default Chamber;
