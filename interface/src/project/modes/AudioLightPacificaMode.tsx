import { FC } from 'react';

import { PacificaModeSettings } from '../types';
import PalettePicker from '../components/PalettePicker';

import { useAudioLightMode, AudioLightModeRendererProps } from './AudioLightMode';

const AudioLightPacificaMode: FC<AudioLightModeRendererProps> = ({ data, updateData }) => {
  const { settings, handleValueChange } = useAudioLightMode<PacificaModeSettings>(data, updateData);

  return (
    <div>
      <PalettePicker
        name="palette1"
        label="Palette 1"
        value={settings.palette1}
        onChange={handleValueChange('palette1')}
      />
      <PalettePicker
        name="palette2"
        label="Palette 2"
        value={settings.palette2}
        onChange={handleValueChange('palette2')}
      />
      <PalettePicker
        name="palette3"
        label="Palette 3"
        value={settings.palette3}
        onChange={handleValueChange('palette3')}
      />
    </div>
  );

};

export default AudioLightPacificaMode;
