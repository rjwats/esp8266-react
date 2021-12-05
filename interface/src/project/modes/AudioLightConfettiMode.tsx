import { FC } from 'react';

import { Box, FormLabel, Slider } from '@mui/material';

import { ConfettiModeSettings } from '../types';
import PalettePicker from '../components/PalettePicker';

import { useAudioLightMode, AudioLightModeRendererProps } from './AudioLightMode';

const AudioLightConfettiMode: FC<AudioLightModeRendererProps> = ({ data, updateData }) => {
  const { settings, handleValueChange, handleSliderChange } = useAudioLightMode<ConfettiModeSettings>(data, updateData);

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
      <Box my={2}>
        <FormLabel>Palette Changes (per cycle)</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.max_changes}
          onChange={handleSliderChange('max_changes')}
        />
        <FormLabel>Brightness</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.brightness}
          onChange={handleSliderChange('brightness')}
        />
        <FormLabel>Delay</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.delay}
          onChange={handleSliderChange('delay')}
        />
      </Box>
    </div>
  );
};

export default AudioLightConfettiMode;
