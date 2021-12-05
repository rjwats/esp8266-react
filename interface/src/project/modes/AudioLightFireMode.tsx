import { FC } from 'react';

import { Box, FormLabel, Slider, Switch } from '@mui/material';

import { FireModeSettings } from '../types';
import PalettePicker from '../components/PalettePicker';

import { useAudioLightMode, AudioLightModeRendererProps } from './AudioLightMode';

const AudioLightFireMode: FC<AudioLightModeRendererProps> = ({ data, updateData }) => {
  const { settings, handleValueChange, handleSliderChange } = useAudioLightMode<FireModeSettings>(data, updateData);

  return (
    <div>
      <PalettePicker
        name="palette"
        label="Palette"
        value={settings.palette}
        onChange={handleValueChange('palette')}
      />
      <Box my={2}>
        <FormLabel>Reverse</FormLabel>
        <Switch
          checked={settings.reverse}
          onChange={handleValueChange('reverse')}
          color="primary"
        />
      </Box>
      <Box my={2}>
        <FormLabel>Cooling</FormLabel>
        <Slider
          min={20}
          max={100}
          step={1}
          value={settings.cooling}
          onChange={handleSliderChange('cooling')}
        />
        <FormLabel>Sparking</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.sparking}
          onChange={handleSliderChange('sparking')}
        />
      </Box>
    </div>
  );
};

export default AudioLightFireMode;
