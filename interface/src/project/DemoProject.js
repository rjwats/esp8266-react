import React, { Component } from 'react';
import { Redirect, Switch } from 'react-router-dom'

import { PROJECT_PATH } from '../constants/Env';
import MenuAppBar from '../components/MenuAppBar';
import AuthenticatedRoute from '../authentication/AuthenticatedRoute';
import AudioLightFrequencyStatus from './AudioLightFrequencyStatus';
import AudioLightSettings from './AudioLightSettings';

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

class DemoProject extends Component {

  handleTabChange = (event, path) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Demo Project">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/demo/frequency`} label="Frequency Status" />
          <Tab value={`/${PROJECT_PATH}/demo/settings`} label="Settings" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/frequency`} component={AudioLightFrequencyStatus} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/settings`} component={AudioLightSettings} />
          <Redirect to={`/${PROJECT_PATH}/demo/frequency`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default DemoProject;
