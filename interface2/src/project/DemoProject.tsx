
import React, { FC } from 'react';
import { Redirect, Route, Switch, useHistory, useRouteMatch } from 'react-router-dom';

import { Tab, Tabs } from '@mui/material';

import { PROJECT_PATH } from '../api/env';

import DemoInformation from './DemoInformation';
import LightStateRestForm from './LightStateRestForm';
import LightMqttSettingsForm from './LightMqttSettingsForm';
import LightStateWebSocketForm from './LightStateWebSocketForm';

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
        <Tab value={`/${PROJECT_PATH}/demo/rest`} label="REST Example" />
        <Tab value={`/${PROJECT_PATH}/demo/socket`} label="WebSocket Example" />
        <Tab value={`/${PROJECT_PATH}/demo/mqtt`} label="MQTT Settings" />
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
        <Route exact path={`/${PROJECT_PATH}/demo/socket`}>
          <LightStateWebSocketForm />
        </Route>
        <Redirect to={`/${PROJECT_PATH}/demo/information`} />
      </Switch>
    </>
  );
};

export default DemoProject;
