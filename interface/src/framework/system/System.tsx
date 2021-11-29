import React, { FC, useContext } from 'react';
import { Redirect, Route, Switch, useHistory, useRouteMatch } from 'react-router-dom';

import { Tab, Tabs } from '@mui/material';

import { AdminRoute, useLayoutTitle } from '../../components';
import { AuthenticatedContext } from '../../contexts/authentication';
import { FeaturesContext } from '../../contexts/features';
import UploadFirmwareForm from './UploadFirmwareForm';
import SystemStatusForm from './SystemStatusForm';
import OTASettingsForm from './OTASettingsForm';

const SystemRouting: FC = () => {
  useLayoutTitle("System");

  const history = useHistory();
  const { url } = useRouteMatch();
  const { me } = useContext(AuthenticatedContext);
  const { features } = useContext(FeaturesContext);

  const handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    history.push(path);
  };

  return (
    <>
      <Tabs value={url} onChange={handleTabChange} variant="fullWidth">
        <Tab value="/system/status" label="System Status" />
        {features.ota && (
          <Tab value="/system/ota" label="OTA Settings" disabled={!me.admin} />
        )}
        {features.upload_firmware && (
          <Tab value="/system/upload" label="Upload Firmware" disabled={!me.admin} />
        )}
      </Tabs>
      <Switch>
        <Route exact path="/system/status">
          <SystemStatusForm />
        </Route>
        {features.ota && (
          <AdminRoute exact path="/system/ota">
            <OTASettingsForm />
          </AdminRoute>
        )}
        {features.upload_firmware && (
          <AdminRoute exact path="/system/upload">
            <UploadFirmwareForm />
          </AdminRoute>
        )}
        <Redirect to="/system/status" />
      </Switch>
    </>
  );

};

const System: FC = () => (
  <Switch>
    <Route exact path={`/system/*`}>
      <SystemRouting />
    </Route>
    <Redirect to="/system/status" />
  </Switch>
);

export default System;
