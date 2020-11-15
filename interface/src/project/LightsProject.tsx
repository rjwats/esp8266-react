import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import SpectrumAnalyzer from './SpectrumAnalyzer';
import AudioLightSettingsController from './AudioLightSettingsController';

class LightsProject extends Component<RouteComponentProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Christmas Lights">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/lights/settings`} label="Lighting Settings" />
          <Tab value={`/${PROJECT_PATH}/lights/spectrum`} label="Spectrum Analyzer" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/lights/settings`} component={AudioLightSettingsController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/lights/spectrum`} component={SpectrumAnalyzer} />
          <Redirect to={`/${PROJECT_PATH}/lights/settings`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default LightsProject;
