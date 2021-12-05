import React, { FC } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RouterTabs, useRouterTab, useLayoutTitle } from '../components';

import PaletteSettingsForm from './PaletteSettingsForm';
import SpectrumAnalyzer from './SpectrumAnalyzer';

const LightsProject: FC = () => {
  useLayoutTitle("Christmas Lights");
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="palettes" label="Palettes" />
        <Tab value="spectrum" label="Spectrum Analyzer" />
      </RouterTabs>
      <Routes>
        <Route path="palettes" element={<PaletteSettingsForm />} />
        <Route path="spectrum" element={<SpectrumAnalyzer />} />
        <Route path="/*" element={<Navigate replace to="spectrum" />} />
      </Routes>
    </>
  );
};

export default LightsProject;

/**
 *
 * <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="scrollable">
          <Tab value={`/${PROJECT_PATH}/lights/settings`} label="Lighting Settings" />
          <Tab value={`/${PROJECT_PATH}/lights/palettes`} label="Palettes" />
          <Tab value={`/${PROJECT_PATH}/lights/led`} label="LED Settings" />
          <Tab value={`/${PROJECT_PATH}/lights/spectrum`} label="Spectrum Analyzer" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/lights/settings`} component={AudioLightSettingsController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/lights/palettes`} component={PaletteSettingsController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/lights/led`} component={LedSettingsController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/lights/spectrum`} component={SpectrumAnalyzer} />
          <Redirect to={`/${PROJECT_PATH}/lights/settings`} />
        </Switch>
 */
