import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { AuthenticatedRoute } from '../authentication';
import { MenuAppBar } from '../components';
import DemoInformation from './DemoInformation';
import DemoController from './DemoController';

class ProjectRouting extends Component<RouteComponentProps>  {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Demo Project">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/information`} label="Demo Information" />
          <Tab value={`/${PROJECT_PATH}/controller`} label="Demo Controller" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/information`} component={DemoInformation} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/controller`} component={DemoController} />
          <Redirect to={`/${PROJECT_PATH}/information`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default ProjectRouting;
