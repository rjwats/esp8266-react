import React, { FC, useContext } from 'react';
import { Navigate, Routes, Route } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RequireAdmin, RouterTabs, useLayoutTitle, useRouterTab } from '../../components';
import { AuthenticatedContext } from '../../contexts/authentication';

import APStatusForm from './APStatusForm';
import APSettingsForm from './APSettingsForm';

const AccessPoint: FC = () => {
  useLayoutTitle("Access Point");

  const authenticatedContext = useContext(AuthenticatedContext);
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="status" label="Access Point Status" />
        <Tab value="settings" label="Access Point Settings" disabled={!authenticatedContext.me.admin} />
      </RouterTabs>
      <Routes>
        <Route path="status" element={<APStatusForm />} />
        <Route
          path="settings"
          element={
            <RequireAdmin>
              <APSettingsForm />
            </RequireAdmin>
          }
        />
        <Route path="/*" element={<Navigate replace to="status" />} />
      </Routes>
    </>
  );

};

export default AccessPoint;
