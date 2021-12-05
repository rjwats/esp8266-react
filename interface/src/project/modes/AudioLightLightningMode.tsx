import { FC } from 'react';

import { Box, FormLabel, Slider } from '@mui/material';

import { LightningModeSettings } from '../types';
import IncludedBands from '../components/IncludedBands';
import ColorPicker from '../components/ColorPicker';

import { AudioLightModeRendererProps, useAudioLightMode } from './AudioLightMode';

const AudioLightLightningMode: FC<AudioLightModeRendererProps> = ({ data, updateData }) => {
  const { settings, handleChange, handleColorChange, handleSliderChange } = useAudioLightMode<LightningModeSettings>(data, updateData);

  return (
    <div>
      <Box my={2}>
        <FormLabel>Color</FormLabel>
        <ColorPicker
          color={settings.color}
          onChange={handleColorChange("color")}
        />
      </Box>
      <Box my={2}>
        <FormLabel>Brightness</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.brightness}
          onChange={handleSliderChange('brightness')}
        />
        <FormLabel>Flashes</FormLabel>
        <Slider
          min={1}
          max={20}
          step={1}
          value={settings.flashes}
          onChange={handleSliderChange('flashes')}
        />
        <FormLabel>Threshold</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.threshold}
          onChange={handleSliderChange('threshold')}
        />
      </Box>
      <Box my={2}>
        <FormLabel>Included Bands</FormLabel>
        <IncludedBands
          value={settings.included_bands}
          onChange={handleChange('included_bands')}
        />
      </Box>
    </div>
  );
};

export default AudioLightLightningMode;
