
import React, { FC } from 'react';
import { Redirect, Route, Switch, useHistory, useRouteMatch } from 'react-router-dom';

import { Tab, Tabs } from '@mui/material';

import { PROJECT_PATH } from '../api/env';

import DemoInformation from './DemoInformation';
import LightStateRestForm from './LightStateRestForm';
import LightMqttSettingsForm from './LightMqttSettingsForm';

const DemoProject: FC = () => {
  const history = useHistory();
  const { url } = useRouteMatch();

  const handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    history.push(path);
  };

  return (
    <>
      <Tabs value={url} onChange={handleTabChange} variant="fullWidth">
        <Tab value={`/${PROJECT_PATH}/demo/information`} label="Information" />
        <Tab value={`/${PROJECT_PATH}/demo/rest`} label="REST Controller" />
        {
          /*
          <Tab value={`/${PROJECT_PATH}/demo/socket`} label="WebSocket Controller" />
          */
        }
        <Tab value={`/${PROJECT_PATH}/demo/mqtt`} label="MQTT Controller" />
      </Tabs>
      <Switch>
        <Route exact path={`/${PROJECT_PATH}/demo/information`}>
          <DemoInformation />
        </Route>
        <Route exact path={`/${PROJECT_PATH}/demo/rest`}>
          <LightStateRestForm />
        </Route>
        <Route exact path={`/${PROJECT_PATH}/demo/mqtt`}>
          <LightMqttSettingsForm />
        </Route>
        {
          /*
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/demo/socket`} component={LightStateWebSocketController} />
           */
        }
        <Redirect to={`/${PROJECT_PATH}/demo/information`} />
      </Switch>
    </>
  );
};

export default DemoProject;
