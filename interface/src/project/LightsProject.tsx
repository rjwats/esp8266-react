import React, { FC } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RouterTabs, useRouterTab, useLayoutTitle } from '../components';

import AudioLightSettingsForm from './AudioLightSettingsForm';
import PaletteSettingsForm from './PaletteSettingsForm';
import LedSettingsForm from './LedSettingsForm';
import SpectrumAnalyzer from './SpectrumAnalyzer';

const LightsProject: FC = () => {
  useLayoutTitle("Christmas Lights");
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="settings" label="Lighting Settings" />
        <Tab value="palettes" label="Palettes" />
        <Tab value="led" label="LED Settings" />
        <Tab value="spectrum" label="Spectrum Analyzer" />
      </RouterTabs>
      <Routes>
        <Route path="settings" element={<AudioLightSettingsForm />} />
        <Route path="palettes" element={<PaletteSettingsForm />} />
        <Route path="led" element={<LedSettingsForm />} />
        <Route path="spectrum" element={<SpectrumAnalyzer />} />
        <Route path="/*" element={<Navigate replace to="settings" />} />
      </Routes>
    </>
  );
};

export default LightsProject;
