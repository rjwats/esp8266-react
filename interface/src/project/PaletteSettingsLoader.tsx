import { FC } from 'react';

import { useRest } from '../utils';

import * as ProjectApi from "./api";
import { PaletteSettings } from './types';
import { PaletteSettingsContext } from './PaletteSettingsContext';
import { FormLoader } from '../components';

const PaletteSettingsLoader: FC = ({ children }) => {
  const { data, setData, loadData, errorMessage } = useRest<PaletteSettings>({ read: ProjectApi.readPaletteSettings });

  if (!data) {
    return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
  }

  return (
    <PaletteSettingsContext.Provider
      value={{
        paletteSettings: data,
        paletteSettingsUpdated: setData
      }}
    >
      {children}
    </PaletteSettingsContext.Provider>
  );
};

export default PaletteSettingsLoader;
