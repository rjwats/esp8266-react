import React, { FC, useContext } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RequireAdmin, RouterTabs, useLayoutTitle, useRouterTab } from '../../components';
import { AuthenticatedContext } from '../../contexts/authentication';

import SerialStatusForm from './SerialStatusForm';
import SerialSettingsForm from './SerialSettingsForm';

const Serial: FC= () => {
  useLayoutTitle("Serial");

  const authenticatedContext = useContext(AuthenticatedContext);
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="status" label="Serial Status" />
        {/* <Tab value="log" label="Remote Log" /> */}
        <Tab value="settings" label="Serial Settings" disabled={!authenticatedContext.me.admin} />
      </RouterTabs>
      <Routes>
        <Route path="status" element={<SerialStatusForm />} />
        <Route
        path="settings"
        element={
          <RequireAdmin>
            <SerialSettingsForm />
          </RequireAdmin>
        }
        />
        <Route path="/*" element={<Navigate replace to="status" />} />
      </Routes>
    </>
  );
};

export default Serial;
